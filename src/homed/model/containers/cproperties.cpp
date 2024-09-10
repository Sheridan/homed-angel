#include "homed/model/containers/cproperties.h"
#include "homed/model/entities/cdevice.h"
#include "homed/model/entities/cendpoint.h"
#include "st.h"

namespace ha
{
namespace homed
{

CProperties::CProperties(CDevice *pDevice, CEndpoint *pEndpoint)
  : CContainer<CProperty>(),
    m_parentDevice(pDevice),
    m_parentEndpoint(pEndpoint)
{}

CProperties::~CProperties() {}

CProperty *CProperties::newElement(const std::string &name)
{
  HA_LOG_VERBOSE("Adding new property: " << m_parentDevice->instance()->name() << ":" << m_parentDevice->name() << ":" << (m_parentDevice != m_parentEndpoint ? m_parentEndpoint->name() + ":" : "") << name);
  return new CProperty(name, m_parentDevice, m_parentEndpoint);
}

}
}
