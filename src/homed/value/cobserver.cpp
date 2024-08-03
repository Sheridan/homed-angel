#include "homed/value/cobserver.h"
#include "homed/model/entities/cproperty.h"
#include "scripting/script/callers/ccallersingle.h"
#include "st.h"

namespace ha
{
namespace homed
{

CObserver::CObserver(const std::string &sn, const std::string &mn, const bool &changedOnly, CProperty *pProperty)
  : m_scriptName(sn),
    m_methodName(mn),
    m_changedOnly(changedOnly),
    m_parentProperty(pProperty)
{
  HA_LOG_VERBOSE("New observer for " << m_scriptName << "->" << m_methodName);
}

CObserver::~CObserver()
{
  HA_LOG_VERBOSE("Releasing observer for " << m_scriptName << "->" << m_methodName);
}

void CObserver::call()
{
  if(!m_changedOnly || m_parentProperty->storage()->valueChanged())
  {
    HA_LOG_VERBOSE("Calling observer for " << m_scriptName << "->" << m_methodName);
    HA_ST->angel()->manager()->script(m_scriptName)->propertyCaller()->append(m_methodName, m_parentProperty);
  }
}
}
}
