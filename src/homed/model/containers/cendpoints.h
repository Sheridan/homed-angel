#pragma once
#include <jsoncpp/json/json.h>
#include "homed/base/ccontainer.h"
#include "mqtt/homed/ctopic.h"
#include "homed/model/entities/cendpoint.h"

namespace ha
{
namespace homed
{

class CDevice;
class CEndpoints : public CContainer<CEndpoint>
{
public:
  explicit CEndpoints(CDevice *pDevice);
  virtual ~CEndpoints();

  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  CDevice *device() { return m_parentDevice; };

private:
  CDevice *m_parentDevice;

  CEndpoint *newElement(const std::string &name) final;
};


}
}
