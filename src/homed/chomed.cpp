#include "homed/chomed.h"
#include "st.h"

namespace ha
{
namespace homed
{

CHomed::CHomed()
{
  HA_ST->mqtt()->clientHomed()->connect();
  HA_ST->mqtt()->clientHomed()->subscribe();
}

CHomed::~CHomed()
{
  for(auto &pair : m_devices)
  {
    delete pair.second;
  }
  m_devices.clear();
}

void CHomed::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  // if(!ready() && topic->topicType() != ha::mqtt::ETopic::tStatus) { return; }
  devices(topic->serviceType())->update(topic, payload);
  m_updated[serviceType2DeviceType(topic->serviceType())] = true;
}

bool CHomed::ready()
{
  return m_updated[dtZigbee] && m_updated[dtCustom];
}

CDevices *CHomed::devices(const EDeviceType &type)
{
  if(!m_devices.contains(type))
  {
    m_devices[type] = new CDevices(type);
  }
  return m_devices[type];
}

CDevices *CHomed::devices(const ha::mqtt::EService &type)
{
  return devices(serviceType2DeviceType(type));
}

CDevice *CHomed::device(const EDeviceType &type, const std::string &deviceName)
{
  return devices(type)->get(deviceName);
}

CDevice *CHomed::device(const std::string &deviceName)
{
  CDevice *d = device(EDeviceType::dtZigbee, deviceName);
  if (d) { return d; }
  return device(EDeviceType::dtCustom, deviceName);
}

CEndpoint *CHomed::endpoint(const EDeviceType &type, const std::string &deviceName, const std::string &endpointName)
{
  CDevice *d = device(type, deviceName);
  if (d) { return d->endpoints()->get(endpointName); }
  return nullptr;
}

CProperty *CHomed::property(const EDeviceType &type, const std::string &deviceName, const std::string &propertyName)
{
  CDevice *d = device(type, deviceName);
  if (d) { return d->properties()->get(propertyName); }
  return nullptr;
}

CProperty *CHomed::property(const EDeviceType &type, const std::string &deviceName, const std::string &endpointName, const std::string &propertyName)
{
  CEndpoint *e = endpoint(type, deviceName, endpointName);
  if(e) { return e->properties()->get(propertyName); }
  return nullptr;
}

EDeviceType CHomed::serviceType2DeviceType(const ha::mqtt::EService &m_serviceType)
{
  switch(m_serviceType)
  {
    case ha::mqtt::EService::sZigbee: return EDeviceType::dtZigbee;
    case ha::mqtt::EService::sCustom: return EDeviceType::dtCustom;
  }
  return EDeviceType::dtUnknown;
}

void CHomed::unsubscribeScript(const std::string &scriptName)
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

void CHomed::publishValue(CDevice *device, CEndpoint *endpoint, CProperty *property, const CValue &value)
{
  HA_ST->mqtt()->publisher()->publish(HA_ST->config()->mqttHomedTopic() + "/td/" + device->typeAsService() + "/" + device->name() + (endpoint ? "/" + endpoint->name() : ""), value.asJson(property->name()).toStyledString());
}

}
}
