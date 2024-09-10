#pragma once
#include "homed/base/cbase.h"
#include "homed/model/containers/cdevices.h"
#include <map>
namespace ha
{
namespace homed
{

class CInstance : virtual public CBase
{
public:
  explicit CInstance(const std::string &name);
  virtual ~CInstance();
  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  const std::string &name() const override { return CBase::name(); }
  bool ready();
  bool real();

  CDevices *devices(const EDeviceType &type);
  CDevices *devices(const ha::mqtt::EService &type);

  void unsubscribeScript(const std::string &scriptName);

  void setJoin(bool enabled);

private:
  std::map<EDeviceType, CDevices *> m_devices;
  std::map<ha::mqtt::EService, std::vector<ha::mqtt::ETopic>> m_updated;

  EDeviceType serviceType2DeviceType(const ha::mqtt::EService &m_serviceType);

  bool ready(const ha::mqtt::EService &type);
};

}
}
