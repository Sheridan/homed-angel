#pragma once
#include <jsoncpp/json/json.h>
#include "homed/base/cbase.h"
#include "homed/model/containers/cproperties.h"
#include "mqtt/homed/ctopic.h"
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

  virtual std::string topicPath(const ha::mqtt::ETopic &topic);
private:
  CProperties *m_properties;
  CDevice *m_parentDevice;

  void updateStatus(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateExpose(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateFd(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateDevice(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  void updateProperty(const std::string &name, const Json::Value &item);
  ha::utils::CStrings findChangedProperties(const Json::Value &payload);

  CProperty* addProperty(const std::string &name, const bool &readonly, const EPropertyValueType &type);
  CProperty* addEnumerateToProperty(CProperty* property, const ha::utils::CStrings &enums);
  CProperty* addEnumerateToProperty(CProperty* property, const Json::Value &enums);
  CProperty* addBordersToProperty(CProperty* property, const double &min, const double &max, const double &step = 1.0);
  CProperty* addBordersToProperty(CProperty* property, const Json::Value &borders);
  CProperty* addUnitToProperty(CProperty* property, const std::string &unit);
};



}
}
