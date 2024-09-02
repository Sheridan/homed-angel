#pragma once
#include <vector>
#include "serial/cserialcommunication.h"

namespace ha
{
namespace serial
{
namespace device
{

class CGsm : public ha::serial::CSerialCommunication
{

public:
  explicit CGsm(const std::string &port, ha::serial::EBaudRate baudRate);
  virtual ~CGsm();

  bool sendSms(const std::string &phone, const std::string &text);
  int         signalLevel();
  std::string operatorName();
  std::string firmwareVersion();
  std::string adapterName();
  std::string imei();
  std::string imsi();
  std::string info();
  std::vector<std::string> supportedEncodings();
  bool ready();
  std::string ussd(const std::string &msg);

};



}
}
}
