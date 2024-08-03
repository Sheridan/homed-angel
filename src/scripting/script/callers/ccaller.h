#pragma once
#include <queue>
#include <string>
#include "scripting/script/cscript.h"

namespace ha
{
namespace scripting
{

// class CScript;
class CCaller
{
public:
  explicit CCaller(CScript *parent);
  virtual ~CCaller();

  virtual void append(const std::string &method);
  virtual void call();

protected:
  CScript *script() { return m_script; };

private:
  std::queue<std::string> m_queue;
  CScript *m_script;
};


}
}
