#include "homed/model/entities/cdevice.h"
#include "st.h"
namespace ha
{
namespace homed
{

CDevice::CDevice(const std::string &name, const EDeviceType &dType, CInstance *pInstance)
  : CBase(name), CEndpoint("common", this),
    m_type(dType),
    m_parentInstance(pInstance)
{
  m_endpoints = new CEndpoints(this);
}

CDevice::~CDevice()
{
  delete m_endpoints;
}

CEndpoint *CDevice::endpoint(const std::string &name)
{
  return m_endpoints->get(name);
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

std::string CDevice::topicPath(const ha::mqtt::ETopic &topic)
{
  std::string path = HA_ST->config()->mqttHomedTopic() + "/" + ha::mqtt::CTopic::topicTypeToString(topic) + "/" + typeAsService() + "/";
  if(instance()->real()) { path += instance()->name() + "/"; }
  return path + name();
}

void CDevice::updateProperties()
{
  std::string topic = HA_ST->config()->mqttHomedTopic() + "/command/" + typeAsService();
  if(instance()->real()) { topic += "/" + instance()->name(); }
  Json::Value command;
  command["action"] = "getProperties";
  command["service"] = "angel";
  command["device"] = name();
  HA_ST->mqtt()->publish(topic, command.toStyledString());
}

}
}
