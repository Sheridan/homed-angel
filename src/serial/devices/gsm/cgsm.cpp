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
  std::lock_guard<std::mutex> lock(m_mutex);

  auto now = std::chrono::steady_clock::now();
  if (now - m_lastSMSSendTime < std::chrono::milliseconds(HA_ST->config()->gsmSMSSendInterval()))
  {
    auto remainingTime = std::chrono::milliseconds(HA_ST->config()->gsmSMSSendInterval()) - (now - m_lastSMSSendTime);
    std::this_thread::sleep_for(remainingTime);
  }

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
  m_lastSMSSendTime = std::chrono::steady_clock::now();
  return true;
}

std::string CGsm::operatorName   () {std::lock_guard<std::mutex> lock(m_mutex); return query("AT+COPS?", ",\"(.+?)\""); }
std::string CGsm::firmwareVersion() {std::lock_guard<std::mutex> lock(m_mutex); return query("AT+GMR"  , "(.*)"      ); }
std::string CGsm::adapterName    () {std::lock_guard<std::mutex> lock(m_mutex); return query("AT+GMM"  , "(.*)"      ); }
std::string CGsm::imei           () {std::lock_guard<std::mutex> lock(m_mutex); return query("AT+GSN"  , "(\\d+)"    ); }
std::string CGsm::imsi           () {std::lock_guard<std::mutex> lock(m_mutex); return query("AT+CIMI" , "(\\d+)"    ); }
std::string CGsm::info           () {std::lock_guard<std::mutex> lock(m_mutex); return query("ATI"     , "(.*)"      ); }

bool                     CGsm::ready             () {std::lock_guard<std::mutex> lock(m_mutex); return isOpen() && isReady() && query("AT+CPAS" , "\\+CPAS:\\s*(\\d+)") == "0"                         ; }
int                      CGsm::signalLevel       () {std::lock_guard<std::mutex> lock(m_mutex); return ha::utils::to_int(                   query("AT+CSQ", "(\\d+),\\d+")                            ); }
std::vector<std::string> CGsm::supportedEncodings() {std::lock_guard<std::mutex> lock(m_mutex); return ha::utils::split (ha::utils::replace(query("AT+CSCS=?", "+CSCS:\\s*\\((.*)\\)"), "\"", ""), ','); }

bool CGsm::busy()
{
  std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
  return !lock.owns_lock();
}

std::string CGsm::ussd(const std::string &msg)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if(!ha::utils::contains(send("AT+CSCS=\"GSM\""          , ha::serial::ECommandEndl::ceCR , 1), "OK")) { HA_LOG_ERR("Can not set encoding"); return ""; }
  if(!ha::utils::contains(send("AT+CUSD=1,\"" + msg + "\"", ha::serial::ECommandEndl::ceCR , 1), "OK")) { HA_LOG_ERR("Can not send ussd query"); return ""; }
  return ha::tools::CIconv::fromGsmUcs2(extract(waitFor("+CUSD"), "\"(.+?)\""));
}

}
}
}
