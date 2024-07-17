#include "homed/model/containers/cdevices.h"
#include "cdevices.h"

namespace ha
{
namespace homed
{

CDevices::CDevices(const EDeviceType &type)
  : CContainer<CDevice>(),
    m_type(type)
{
}

CDevices::~CDevices()
{
}

void CDevices::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  if(!topic->deviceEndpoint().empty())
  {
    ensure(topic->device())->endpoints()->ensure(topic->deviceEndpoint())->update(topic, payload);
  }
  else
  {
    ensure(topic->device())->update(topic, payload);
  }
}

CDevice *CDevices::newElement(const std::string &name)
{
  return new CDevice(name, m_type);
}

}
}
