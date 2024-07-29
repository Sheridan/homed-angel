#include "homed/model/entities/cproperty.h"
#include "homed/model/entities/cdevice.h"
#include "homed/model/entities/cendpoint.h"
#include "st.h"


namespace ha
{
namespace homed
{

CProperty::CProperty(const std::string &name, CDevice *pDevice, CEndpoint *pEndpoint)
  : CBase(name),
    m_parentDevice(pDevice),
    m_parentEndpoint(pEndpoint),
    m_storage(new CStorage(this))
{}

CProperty::~CProperty()
{}

CStorage  *CProperty::storage () { return m_storage; }

const CValue     &CProperty::last   () const { return m_storage->last(); }
const std::string CProperty::type   () const { return m_storage->type(); }
CValues           CProperty::history()       { return m_storage->list(); }

void CProperty::subscribe(const std::string &scriptName, const std::string &methodName, const bool &changedOnly)
{
  storage()->addObserver(scriptName, methodName, changedOnly);
}

void CProperty::set(const std::string &value) { set(CValue(value, storage())); }
void CProperty::set(const int         &value) { set(CValue(value, storage())); }
void CProperty::set(const double      &value) { set(CValue(value, storage())); }
void CProperty::set(const bool        &value) { set(CValue(value, storage())); }
void CProperty::set(const CColor      &value) { set(CValue(value, storage())); }
void CProperty::set(const CValue      &value)
{
  // HA_LOG_NFO("New value: " << value.asString());
  if(readonly())
  {
    HA_LOG_WRN("Property " << name() << " is readonly");
    return;
  }
  HA_ST->homed()->publishValue(device(), endpoint(), this, value);
}


}
}
