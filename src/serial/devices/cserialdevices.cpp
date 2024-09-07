#include "serial/devices/cserialdevices.h"
#include "utils/fs.h"
#include "st.h"
namespace ha
{
namespace serial
{
namespace device
{

CSerialDevices::CSerialDevices()
  : m_gsm(nullptr)
{}

CSerialDevices::~CSerialDevices()
{}

bool CSerialDevices::gsmAccessible() const
{
  if(ha::utils::fileExists(HA_ST->config()->gsmDevice()))
  {
    if(ha::serial::CSerial::intToBaudRate(HA_ST->config()->gsmBaudrate()) != ha::serial::EBaudRate::brUnknown)
    {
      return true;
    } else { HA_LOG_ERR("Wrong " << HA_ST->config()->gsmDevice() << " baudrate: " << HA_ST->config()->gsmBaudrate()); }
  } else { HA_LOG_ERR("Device file " << HA_ST->config()->gsmDevice() << " does not exist"); }
  return false;
}

bool CSerialDevices::gsmAvailable() const
{
  return m_gsm != nullptr;
}

CGsm *CSerialDevices::gsm()
{
  if(!gsmAvailable() && gsmAccessible())
  {
    HA_LOG_VERBOSE("Initializing GSM");
    m_gsm = new CGsm(HA_ST->config()->gsmDevice(), ha::serial::CSerial::intToBaudRate(HA_ST->config()->gsmBaudrate()));
  }
  return m_gsm;
}

void CSerialDevices::start()
{
  gsm();
}

void CSerialDevices::stop()
{
  delete m_gsm; m_gsm = nullptr;
}

}
}
}
