#include "homed/chomed.h"
#include "st.h"
#include "chomed.h"

namespace ha
{
namespace homed
{

CHomed::CHomed()
{}

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
  HA_ST->mqtt()->publish(HA_ST->config()->mqttHomedTopic() + "/td/" + device->typeAsService() + "/" + device->name() + (endpoint ? "/" + endpoint->name() : ""), value.asJson(property->name()).toStyledString());
}

// #ifdef HA_DEBUG
// void CHomed::dump()
// {
//   for(const std::string &deviceName : listDeviceZigbee())
//   {
//     HA_LOG_DBG(deviceName);
//     CDeviceZigbee *device = getDeviceZigbee(deviceName);
//     for(const std::string &propertyName : device->listProperty())
//     {
//       dumpProperty(device->getProperty(propertyName));
//     }
//     for(const std::string &endpointName : device->listEndpoint())
//     {
//       HA_LOG_DBG(" " << endpointName);
//       CEndpoint *endpoint = device->getEndpoint(endpointName);
//       for(const std::string &propertyName : endpoint->listProperty())
//       {
//         dumpProperty(endpoint->getProperty(propertyName));
//       }
//     }
//   }
// }

// void CHomed::dumpProperty(CProperty *property)
// {
//   HA_LOG_DBG("  " <<  property->name() << ": "
//                   << (property->readonly() ? "readonly, " : "")
//                   << (property->unit().empty() ? "" : "unit: " + property->unit() + ", ")
//                   << (property->min () != 0 ? "min: "  + std::to_string(property->min())  + ", " : "")
//                   << (property->max () != 0 ? "max: "  + std::to_string(property->max())  + ", " : "")
//                   << (property->step() != 0 ? "step: " + std::to_string(property->step()) + ", " : "")
//                   << "vtype: " << property->valueType());
//   if(!property->enumerate().empty())
//   {
//     HA_LOG_DBG("   enumerate:");
//     for(const std::string &en : property->enumerate())
//     {
//       HA_LOG_DBG("    " << en);
//     }
//   }
// }

// #endif // DEBUG
}
}
