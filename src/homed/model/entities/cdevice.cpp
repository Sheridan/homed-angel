#include "homed/model/entities/cdevice.h"
#include "cdevice.h"

namespace ha
{
namespace homed
{

CDevice::CDevice(const std::string &name, const EDeviceType &dType)
  : CBase(name), CEndpoint("common", this),
    m_type(dType)
{
  m_endpoints = new CEndpoints(this);
}

CDevice::~CDevice()
{
  delete m_endpoints;
}

const std::string CDevice::typeAsService()
{
  switch(m_type)
  {
    case EDeviceType::dtZigbee: return "zigbee";
    case EDeviceType::dtCustom: return "custom";
  }
  return "unknown";
}
}
}
