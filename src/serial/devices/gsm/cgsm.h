#pragma once
#include <vector>
#include <mutex>
#include <chrono>
#include "serial/cserialcommunication.h"

namespace ha
{
namespace serial
{
namespace device
{

struct SSms
{
  std::string phone;
  std::string text;
};

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
  std::string ussd(const std::string &msg);
  std::vector<std::string> supportedEncodings();
  bool ready();
  bool busy();


private:
  std::mutex m_mutex;
  std::chrono::steady_clock::time_point m_lastSMSSendTime;

};

}
}
}
