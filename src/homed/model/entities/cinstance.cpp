#include "homed/model/entities/cinstance.h"

namespace ha
{
namespace homed
{

CInstance::CInstance(const std::string &name)
  : CBase(name)
{}

CInstance::~CInstance()
{
  for(auto &pair : m_devices)
  {
    delete pair.second;
  }
  m_devices.clear();
}

void CInstance::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  devices(topic->serviceType())->update(topic, payload);
  m_updated[serviceType2DeviceType(topic->serviceType())] = true;
}

bool CInstance::ready()
{
  return m_updated[dtZigbee] && m_updated[dtCustom];
}

bool CInstance::real()
{
  return !name().empty();
}

CDevices *CInstance::devices(const EDeviceType &type)
{
  if(!m_devices.contains(type))
  {
    m_devices[type] = new CDevices(type, this);
  }
  return m_devices[type];
}

CDevices *CInstance::devices(const ha::mqtt::EService &type)
{
  return devices(serviceType2DeviceType(type));
}

EDeviceType CInstance::serviceType2DeviceType(const ha::mqtt::EService &m_serviceType)
{
  switch(m_serviceType)
  {
    case ha::mqtt::EService::sZigbee: return EDeviceType::dtZigbee;
    case ha::mqtt::EService::sCustom: return EDeviceType::dtCustom;
  }
  return EDeviceType::dtUnknown;
}

void CInstance::unsubscribeScript(const std::string &scriptName)
{
  for(auto &pair : m_devices)
  {
    for(CDevice *d : pair.second->items())
    {
      for(CEndpoint *e : d->endpoints()->items())
      {
        for(CProperty *p : e->properties()->items())
        {
          p->storage()->removeObserversForScript(scriptName);
        }
      }
      for(CProperty *p : d->properties()->items())
      {
        p->storage()->removeObserversForScript(scriptName);
      }
    }
  }
}

}
}
