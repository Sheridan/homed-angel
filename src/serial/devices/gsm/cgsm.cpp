#include "serial/devices/gsm/cgsm.h"
#include "utils/string.h"
#include "utils/numeric.h"
#include "utils/thread.h"
#include "tools/iconv/ciconv.h"
#include "st.h"

namespace ha
{
namespace serial
{
namespace device
{

// AT+CMGL=4 - read all sms,  AT+CMGDA=6 - del all sms

CGsm::CGsm(const std::string &port, ha::serial::EBaudRate baudRate)
  : ha::serial::CSerialCommunication(port, baudRate, ha::serial::EParity::pNone, ha::serial::EStopBits::sbOne, ha::serial::EFlowControl::fcNone, ha::serial::EBitsPerByte::bpb8)
{}

CGsm::~CGsm()
{}

bool CGsm::sendSms(const std::string &phone, const std::string &text)
{

  std::string encodedText = "", encodedPhone = "";
  try
  {
    encodedPhone = ha::tools::CIconv::toGsmUcs2(phone);
    encodedText  = ha::tools::CIconv::toGsmUcs2(text );
    // HA_LOG_NFO("phone: " << encodedPhone << ", text: " << encodedText);
  }
  catch(const std::runtime_error& error)
  {
    HA_LOG_ERR("Can not send sms, encoding failed: " << error.what());
    return false;
  }
  if(!ha::utils::contains(send("AT+CMGF=1"                        , ha::serial::ECommandEndl::ceCR , "OK"), "OK")) { HA_LOG_ERR("Can not set text mode"); return false; }
  if(!ha::utils::contains(send("AT+CSMP=17,167,2,25"              , ha::serial::ECommandEndl::ceCR , "OK"), "OK")) { HA_LOG_ERR("Can not set sms options"); return false; }
  if(!ha::utils::contains(send("AT+CSCS=\"UCS2\""                 , ha::serial::ECommandEndl::ceCR , "OK"), "OK")) { HA_LOG_ERR("Can not set encoding"); return false; }
                          send("AT+CMGS=\"" + encodedPhone +  "\"", ha::serial::ECommandEndl::ceCR, 0); ha::utils::sleep(50);
  if(!ha::utils::contains(send(encodedText                        , ha::serial::ECommandEndl::ceSUB, "OK"), "OK")) { HA_LOG_ERR("Can not send sms"); return false; }
  return true;
}

std::string CGsm::operatorName   () { return query("AT+COPS?", ",\"(.+?)\""); }
std::string CGsm::firmwareVersion() { return query("AT+GMR"  , "(.*)"      ); }
std::string CGsm::adapterName    () { return query("AT+GMM"  , "(.*)"      ); }
std::string CGsm::imei           () { return query("AT+GSN"  , "(\\d+)"    ); }
std::string CGsm::imsi           () { return query("AT+CIMI" , "(\\d+)"    ); }
std::string CGsm::info           () { return query("ATI"     , "(.*)"      ); }

bool                     CGsm::ready             () { return isOpen() && isReady() && query("AT+CPAS" , "\\+CPAS:\\s*(\\d+)") == "0"                         ; }
int                      CGsm::signalLevel       () { return ha::utils::to_int(                   query("AT+CSQ", "(\\d+),\\d+")                            ); }
std::vector<std::string> CGsm::supportedEncodings() { return ha::utils::split (ha::utils::replace(query("AT+CSCS=?", "+CSCS:\\s*\\((.*)\\)"), "\"", ""), ','); }

std::string CGsm::ussd(const std::string &msg)
{
  if(!ha::utils::contains(send("AT+CSCS=\"GSM\""          , ha::serial::ECommandEndl::ceCR , 1), "OK")) { HA_LOG_ERR("Can not set encoding"); return ""; }
  if(!ha::utils::contains(send("AT+CUSD=1,\"" + msg + "\"", ha::serial::ECommandEndl::ceCR , 1), "OK")) { HA_LOG_ERR("Can not send ussd query"); return ""; }
  return ha::tools::CIconv::fromGsmUcs2(extract(waitFor("+CUSD"), "\"(.+?)\""));
}

}
}
}
