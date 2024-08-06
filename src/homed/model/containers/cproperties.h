#pragma once
#include <jsoncpp/json/json.h>
#include "homed/model/entities/cproperty.h"
#include "homed/base/ccontainer.h"
#include "mqtt/homed/ctopic.h"

namespace ha
{
namespace homed
{

class CDevice;
class CEndpoint;

class CProperties : public CContainer<CProperty>
{
public:
  explicit CProperties(CDevice *pDevice, CEndpoint *pEndpoint = nullptr);
  virtual ~CProperties();

  CDevice *device() { return m_parentDevice; };
  CEndpoint *endpoint() { return m_parentEndpoint; };

private:
  CDevice *m_parentDevice;
  CEndpoint *m_parentEndpoint;

  CProperty *newElement(const std::string &name) final;
};

}
}
