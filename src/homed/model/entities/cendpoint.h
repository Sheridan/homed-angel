#pragma once
#include <jsoncpp/json/json.h>
#include "homed/base/cbase.h"
#include "homed/model/containers/cproperties.h"
#include "mqtt/ctopic.h"
#include "utils/string.h"


namespace ha
{
namespace homed
{

class CDevice;
class CEndpoint : virtual public CBase
{
public:
  explicit CEndpoint(const std::string &name, CDevice *pDevice);
  virtual ~CEndpoint();
  const std::string &name() const override { return CBase::name(); }

  CProperties *properties() { return m_properties; }
  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);

  void setParentDevice(CDevice *pDevice) { m_parentDevice = pDevice; };
  CDevice *device() { return m_parentDevice; };
private:
  CProperties *m_properties;
  CDevice *m_parentDevice;

  void updateStatus(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateExpose(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateFd(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateDevice(const ha::mqtt::CTopic *topic, const Json::Value &payload);

  void updateProperty(const std::string &name, const Json::Value &item);

  ha::utils::CStrings findChangedProperties(const Json::Value &payload);
};



}
}
