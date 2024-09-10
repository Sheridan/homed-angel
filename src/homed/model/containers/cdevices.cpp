#include "homed/model/containers/cdevices.h"
#include "st.h"


namespace ha
{
namespace homed
{

CDevices::CDevices(const EDeviceType &type, CInstance *pInstance)
  : CContainer<CDevice>(),
    m_type(type),
    m_parentInstance(pInstance)
{}

CDevices::~CDevices()
{}

void CDevices::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  bool needUpdateProperties = !exists(topic->device());
  CDevice *d = ensure(topic->device());
  if(!topic->deviceEndpoint().empty()) { d->endpoints()->ensure(topic->deviceEndpoint())->update(topic, payload); }
  else                                 { d                                              ->update(topic, payload); }
  if(needUpdateProperties) { d->updateProperties(); }
}

CDevice *CDevices::newElement(const std::string &name)
{
  HA_LOG_VERBOSE("Adding new device: " << m_parentInstance->name() << ":" << name);
  return new CDevice(name, m_type, m_parentInstance);
}

}
}
