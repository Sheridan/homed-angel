#pragma once
#include <jsoncpp/json/json.h>
#include "mqtt/ctopic.h"

#include "homed/base/ccontainer.h"
#include "homed/model/containers/cdevices.h"

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
  // #ifdef HA_DEBUG
  // void dump();
  // void dumpProperty(CProperty *property);
  // #endif // DEBUG
  CDevices *devices(const EDeviceType &type);
  CDevices *devices(const ha::mqtt::EService &type);
  CDevice  *device(const std::string &name);
  void unsubscribeScript(const std::string &scriptName);

  void publishValue(CDevice *device, CEndpoint *endpoint, CProperty *property, const CValue &value);

private:
  std::map<EDeviceType, CDevices *> m_devices;
  std::map<EDeviceType, bool> m_updated;
  EDeviceType serviceType2DeviceType(const ha::mqtt::EService &m_serviceType);

};

}
}
