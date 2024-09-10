#include "homed/model/containers/cendpoints.h"
#include "homed/model/entities/cdevice.h"
#include "st.h"


namespace ha
{
namespace homed
{

CEndpoints::CEndpoints(CDevice *pDevice)
  : CContainer<CEndpoint>(),
    m_parentDevice(pDevice)
{}

CEndpoints::~CEndpoints() {}

void CEndpoints::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  ensure(topic->deviceEndpoint())->update(topic, payload);
}

CEndpoint *CEndpoints::newElement(const std::string &name)
{
  HA_LOG_VERBOSE("Adding new endpoint: " << m_parentDevice->instance()->name() << ":" << m_parentDevice->name() << ":" << name);
  return new CEndpoint(name, m_parentDevice);
}

}
}
