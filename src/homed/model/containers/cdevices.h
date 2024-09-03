#pragma once
#include <jsoncpp/json/json.h>
#include "homed/model/entities/cdevice.h"
#include "homed/base/ccontainer.h"
#include "mqtt/homed/ctopic.h"

namespace ha
{
namespace homed
{

class CInstance;
class CDevices : public CContainer<CDevice>
{
public:
  explicit CDevices(const EDeviceType &type, CInstance *pInstance);
  virtual ~CDevices();

  const EDeviceType &type() const { return m_type; }
  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  CInstance *instance() { return m_parentInstance; };

private:
  EDeviceType m_type;
  CInstance *m_parentInstance;

  CDevice *newElement(const std::string &name) final;
};


}
}
