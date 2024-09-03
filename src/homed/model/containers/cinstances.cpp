#include "homed/model/containers/cinstances.h"
#include "cinstances.h"

namespace ha
{
namespace homed
{

CInstances::CInstances()
  : CContainer<CInstance>()
{}

CInstances::~CInstances()
{}

void CInstances::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  ensure(topic->instance())->update(topic, payload);
}

bool CInstances::ready()
{
  for(CInstance *instance : items())
  {
    if(!instance->ready()) { return false; }
  }
  return true;
}

CDevice* CInstances::device(const std::string &deviceName)
{
  CDevice *d = device(EDeviceType::dtZigbee, deviceName);
  if (d) { return d; }
  return device(EDeviceType::dtCustom, deviceName);
}

CDevice* CInstances::device(const EDeviceType &type, const std::string &deviceName)
{
  CDevice *d = nullptr;
  for(CInstance *instance : items())
  {
    d = instance->devices(type)->get(deviceName);
    if(d) { return d; }
  }
  return d;
}

CInstance *CInstances::newElement(const std::string &name)
{
  return new CInstance(name);
}

}
}
