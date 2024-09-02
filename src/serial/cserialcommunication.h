#pragma once
#include "serial/cserial.h"
#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace serial
{

class CSerialCommunication : public CSerial, public ha::scripting::CScriptObject
{
public:
  explicit CSerialCommunication(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte);
  virtual ~CSerialCommunication();
  std::string send(const std::string &command, const ECommandEndl &endl, int timeout);
  std::string send(const std::string &command, const ECommandEndl &endl, const std::string &needle);
  std::string query(const std::string &command, const std::string &regex);
  bool isReady();

  CSerialCommunication           (const CSerialCommunication &other) = delete;
  CSerialCommunication& operator=(const CSerialCommunication &other) = delete;

protected:
  std::string extract(const std::string &input, const std::string &regex);
};

}
}
