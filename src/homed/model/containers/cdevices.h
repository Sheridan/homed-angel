#pragma once
#include <jsoncpp/json/json.h>
#include "homed/model/entities/cdevice.h"
#include "homed/base/ccontainer.h"
#include "mqtt/ctopic.h"

namespace ha
{
namespace homed
{

class CDevices : public CContainer<CDevice>
{
public:
  explicit CDevices(const EDeviceType &type);
  virtual ~CDevices();

  const EDeviceType &type() const { return m_type; }
  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);

private:
  EDeviceType m_type;

  CDevice *newElement(const std::string &name) final;
};


}
}
