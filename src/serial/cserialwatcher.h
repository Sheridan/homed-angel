#pragma once
#include "serial/cserial.h"
#include "scripting/script/cscriptobject.h"
#include <thread>
#include <mutex>

namespace ha
{
namespace serial
{

class CSerialWatcher : public CSerial, public ha::scripting::CScriptObject
{
public:
  explicit CSerialWatcher(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte);
  virtual ~CSerialWatcher();

  void start();
  void stop();

  CSerialWatcher           (const CSerialWatcher &other) = delete;
  CSerialWatcher& operator=(const CSerialWatcher &other) = delete;


private:
  void watch();

  std::thread m_thread;
  bool m_running;
};

}
}
