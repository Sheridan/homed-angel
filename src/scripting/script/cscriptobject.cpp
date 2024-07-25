#include "scripting/script/cscriptobject.h"
#include "st.h"
namespace ha
{
namespace scripting
{

CScriptObject::CScriptObject()
  : m_refCount(1),
    m_gcFlag(false)
{
  HA_LOG_DBG_SCRIPT("CScriptObject construct");
}

CScriptObject::~CScriptObject()
{
  HA_LOG_DBG_SCRIPT("CScriptObject destruct");
}

void CScriptObject::addRef()
{
  m_refCount++;
}

void CScriptObject::release()
{
  if (--m_refCount == 0) { delete this; }
}

int  CScriptObject::getRefCount() { return m_refCount; }
void CScriptObject::setGCFlag  () { m_gcFlag = true;   }
bool CScriptObject::getGCFlag  () { return m_gcFlag;   }
void CScriptObject::enumRefs   (asIScriptEngine *engine) {}
void CScriptObject::releaseRefs(asIScriptEngine *engine) {}

}
}
