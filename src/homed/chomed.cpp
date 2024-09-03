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
  m_instances = new CInstances();
}

CHomed::~CHomed()
{
  delete m_instances;
}

void CHomed::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  m_instances->update(topic, payload);
}

bool CHomed::ready()
{
  return m_instances->ready();
}

CInstances *CHomed::instances()
{
  return m_instances;
}

CInstance *CHomed::instance(const std::string &name)
{
  return m_instances->get(name);
}

CDevice *CHomed::device(const EDeviceType &type, const std::string &deviceName)
{
  return m_instances->device(type, deviceName);
}

CDevice *CHomed::device(const std::string &deviceName)
{
  return m_instances->device(deviceName);
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

void CHomed::unsubscribeScript(const std::string &scriptName)
{
  for(CInstance *i : m_instances->items())
  {
    i->unsubscribeScript(scriptName);
  }
}

void CHomed::publishValue(CDevice *device, CEndpoint *endpoint, CProperty *property, const CValue &value)
{
  std::string path = endpoint == device ? device->topicPath(ha::mqtt::ETopic::tTd) : endpoint->topicPath(ha::mqtt::ETopic::tTd);
  HA_ST->mqtt()->publisher()->publish(path, value.asJson(property->name()).toStyledString());
}

}
}
