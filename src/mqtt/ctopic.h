#pragma once
#include <string>
#include "defines/class_helpers.h"

namespace ha
{
namespace mqtt
{

enum ETopic
{
  tCommand,
  tDevice,
  tEvent,
  tExpose,
  tFd,
  tService,
  tStatus,
  tTd,
  tUnknown
};

enum EService
{
  sZigbee,
  sCustom,
  sWeb,
  sCloud,
  sUnknown
};

class CTopic
{
public:
  CTopic(const std::string &topic);
  CTopic(CTopic *topic, const std::string &device, const std::string &endpoint = "");
  ~CTopic();

  HA_SIMPLE_GETTER(ETopic     , topicType     );
  HA_SIMPLE_GETTER(EService   , serviceType   );
  HA_SIMPLE_GETTER(std::string, root          );
  HA_SIMPLE_GETTER(std::string, topic         );
  HA_SIMPLE_GETTER(std::string, service       );
  HA_SIMPLE_GETTER(std::string, device        );
  HA_SIMPLE_GETTER(std::string, deviceEndpoint);

private:
  std::string m_root;
  std::string m_topic;
  std::string m_service;
  std::string m_device;
  std::string m_deviceEndpoint;
  ETopic m_topicType;
  EService m_serviceType;

  void parseTopic(const std::string &topic);
  ETopic mineTopicType();
  EService mineServiceType();
};


}
}
