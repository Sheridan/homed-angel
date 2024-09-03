#pragma once
#include <jsoncpp/json/json.h>
#include "mqtt/homed/ctopic.h"

#include "homed/model/entities/cdevice.h"
#include "homed/model/containers/cinstances.h"

#include <map>

namespace ha
{
namespace homed
{

class CHomed
{

public:
   CHomed();
  ~CHomed();
  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  bool ready();

  CInstances *instances();
  CInstance  *instance (const std::string &name);
  CDevice    *device   (                         const std::string &deviceName);
  CDevice    *device   (const EDeviceType &type, const std::string &deviceName);
  CEndpoint  *endpoint (const EDeviceType &type, const std::string &deviceName, const std::string &endpointName);
  CProperty  *property (const EDeviceType &type, const std::string &deviceName                                 , const std::string &propertyName);
  CProperty  *property (const EDeviceType &type, const std::string &deviceName, const std::string &endpointName, const std::string &propertyName);

  void unsubscribeScript(const std::string &scriptName);
  void publishValue(CDevice *device, CEndpoint *endpoint, CProperty *property, const CValue &value);

private:
  CInstances *m_instances;

};

}
}
