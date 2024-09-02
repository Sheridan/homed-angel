#include "serial/cserialcommunication.h"
#include "utils/string.h"
#include "st.h"

namespace ha
{
namespace serial
{

CSerialCommunication::CSerialCommunication(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte)
  : CSerial(port, baudRate, parity, stopBits, flowControl, bitsPerByte)
{}

CSerialCommunication::~CSerialCommunication()
{}

bool CSerialCommunication::isReady()
{
  return ha::utils::contains(send("AT", ha::serial::ECommandEndl::ceCR , 1), "OK");
}


std::string CSerialCommunication::send(const std::string &command, const ECommandEndl &endl, int timeout)
{
  if(write(command, endlToString(endl)))
  {
    return read(timeout);
  }
  return "";
}

std::string CSerialCommunication::send(const std::string &command, const ECommandEndl &endl, const std::string &needle)
{
  if(write(command, endlToString(endl)))
  {
    return waitFor(needle);
  }
  return "";
}

std::string CSerialCommunication::query(const std::string &command, const std::string &regex)
{
  return extract(send(command, ha::serial::ECommandEndl::ceCR, 4), regex);
}

std::string CSerialCommunication::extract(const std::string &input, const std::string &regex)
{
  return ha::utils::extract(input, "((\\s*OK\\s*)?" + regex + "(\\s*OK\\s*)?)", 3);
}

}
}
