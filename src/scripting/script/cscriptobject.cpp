#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace scripting
{

CScriptObject::CScriptObject()
  : m_refCount(1),
    m_gcFlag(false)
{}

CScriptObject::~CScriptObject()
{}

void CScriptObject::addRef()
{
  m_refCount++;
}

void CScriptObject::release()
{
  if (--m_refCount == 0) { delete this; }
}

int  CScriptObject::getRefCount() { return m_refCount; }
void CScriptObject::setGCFlag() { m_gcFlag = true; }
bool CScriptObject::getGCFlag() { return m_gcFlag; }
void CScriptObject::enumRefs(asIScriptEngine *engine) {}
void CScriptObject::releaseRefs(asIScriptEngine *engine) {}

}
}
