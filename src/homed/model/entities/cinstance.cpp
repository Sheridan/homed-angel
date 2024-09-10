#include "homed/model/entities/cinstance.h"
#include "st.h"

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
  if(!ready())
  {
    switch(topic->topicType())
    {
      case ha::mqtt::ETopic::tExpose:
      case ha::mqtt::ETopic::tStatus: break;
      default: return;
    }
  }
  devices(topic->serviceType())->update(topic, payload);
  m_updated[topic->serviceType()].push_back(topic->topicType());
}

bool CInstance::ready(const ha::mqtt::EService &type)
{
  if(m_updated.contains(type) && !m_updated[type].empty())
  {
    auto v = m_updated[type];
    return std::find(v.begin(), v.end(), ha::mqtt::ETopic::tStatus) != v.end() &&
           std::find(v.begin(), v.end(), ha::mqtt::ETopic::tExpose) != v.end();
  }
  return false;
}

bool CInstance::ready()
{
  return ready(ha::mqtt::EService::sZigbee) || ready(ha::mqtt::EService::sCustom);
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

void CInstance::setJoin(bool enabled)
{
  if(ready(ha::mqtt::EService::sZigbee))
  {
    std::string topic = HA_ST->config()->mqttHomedTopic() + "/command/zigbee/" + name();
    Json::Value command;
    command["action" ] = "setPermitJoin";
    command["enabled"] = enabled;
    HA_ST->mqtt()->publish(topic, command.toStyledString());
  }
}

}
}
