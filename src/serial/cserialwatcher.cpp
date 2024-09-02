#include "serial/cserialwatcher.h"
#include "st.h"

namespace ha
{
namespace serial
{

CSerialWatcher::CSerialWatcher(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte)
  : CSerial(port, baudRate, parity, stopBits, flowControl, bitsPerByte), m_running(false)
{}

CSerialWatcher::~CSerialWatcher()
{
  stop();
}

void CSerialWatcher::start()
{
  m_running = true;
  m_thread = std::thread(&CSerialWatcher::watch, this);
}

void CSerialWatcher::stop()
{
  m_running = false;
  if (m_thread.joinable())
  {
    m_thread.join();
  }
}

void CSerialWatcher::watch()
{
  char buffer[256];
  while (m_running)
  {
    HA_LOG_DBG_SERIAL(read(10));
  }
}

}
}
