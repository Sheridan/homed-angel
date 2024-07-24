#pragma once
#include <angelscript.h>

namespace ha
{
namespace scripting
{

class CScriptObject
{
public:
  explicit CScriptObject(unsigned int rc = 1);
  virtual ~CScriptObject();

  void addRef();
  void release();

  int  getRefCount();
  void setGCFlag();
  bool getGCFlag();
  void enumRefs(asIScriptEngine *engine);
  void releaseRefs(asIScriptEngine *engine);

private:
  unsigned int m_refCount;
  bool m_gcFlag;

};

}
}
