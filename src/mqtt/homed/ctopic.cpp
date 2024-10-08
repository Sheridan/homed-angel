#include "mqtt/homed/ctopic.h"
#include "st.h"

#include <sstream>
#include <iostream>

namespace ha
{
namespace mqtt
{

CTopic::CTopic(const std::string &topic)
  : m_root(HA_ST->config()->mqttHomedTopic()),
    m_topic(""),
    m_service(""),
    m_instance(""),
    m_device(""),
    m_deviceEndpoint("")
{
  parseTopic(topic);
  m_topicType = mineTopicType();
  m_serviceType = mineServiceType();

  // HA_LOG_DBG(m_root << "/" << m_topic << "/" << m_service << "/" << m_device << "/" << m_deviceEndpoint << " : " << m_topicType);
}

CTopic::CTopic(CTopic *topic, const std::string &device, const std::string &endpoint)
  : m_root(topic->root()),
    m_topic(topic->topic()),
    m_service(topic->service()),
    m_instance(topic->instance()),
    m_device(device),
    m_deviceEndpoint(endpoint),
    m_topicType(topic->topicType()),
    m_serviceType(topic->serviceType())
{}

CTopic::~CTopic() {}

void CTopic::parseTopic(const std::string &topic)
{
  std::string root;
  std::istringstream stream(topic);
  std::getline(stream, root            , '/');
  std::getline(stream, m_topic         , '/');
  std::getline(stream, m_service       , '/');
  if(m_service == "zigbee")
  {
    std::getline(stream, m_instance    , '/');
  }
  std::getline(stream, m_device        , '/');
  std::getline(stream, m_deviceEndpoint, '/');
}

ETopic CTopic::mineTopicType()
{
  if(m_topic == "command") { return ETopic::tCommand; }
  if(m_topic == "device" ) { return ETopic::tDevice;  }
  if(m_topic == "event"  ) { return ETopic::tEvent;   }
  if(m_topic == "expose" ) { return ETopic::tExpose;  }
  if(m_topic == "fd"     ) { return ETopic::tFd;      }
  if(m_topic == "service") { return ETopic::tService; }
  if(m_topic == "status" ) { return ETopic::tStatus;  }
  if(m_topic == "td"     ) { return ETopic::tTd;      }
  return ETopic::tUnknown;
}

EService CTopic::mineServiceType()
{
  if(m_service == "zigbee") { return EService::sZigbee; }
  if(m_service == "custom") { return EService::sCustom; }
  if(m_service == "web"   ) { return EService::sWeb;    }
  if(m_service == "cloud" ) { return EService::sCloud;  }
  return EService::sUnknown;
}

std::string CTopic::topicTypeToString(const ETopic &tType)
{
  switch(tType)
  {
    case ETopic::tCommand: return "command";
    case ETopic::tDevice:  return "device";
    case ETopic::tEvent:   return "event";
    case ETopic::tExpose:  return "expose";
    case ETopic::tFd:      return "fd";
    case ETopic::tService: return "service";
    case ETopic::tStatus:  return "status";
    case ETopic::tTd:      return "td";
  }
  return "unknown";
}

}
}
