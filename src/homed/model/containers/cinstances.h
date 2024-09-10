#pragma once
#include <jsoncpp/json/json.h>
#include "homed/model/entities/cinstance.h"
#include "homed/base/ccontainer.h"
#include "mqtt/homed/ctopic.h"



namespace ha
{
namespace homed
{

class CInstances : public CContainer<CInstance>
{
public:
  explicit CInstances();
  virtual ~CInstances();

  void update(const ha::mqtt::CTopic *topic, const Json::Value &payload);
  bool ready();

  CDevice* device(                         const std::string &deviceName);
  CDevice* device(const EDeviceType &type, const std::string &deviceName);

  void setJoin(bool enabled);

private:

  CInstance *newElement(const std::string &name) final;
};


}
}
