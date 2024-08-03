#include "scripting/script/callers/ccaller.h"
#include "scripting/script/cscript.h"

namespace ha
{
namespace scripting
{

CCaller::CCaller(CScript *parent)
  : m_script(parent)
{}

CCaller::~CCaller()
{}

void CCaller::append(const std::string &method)
{
  m_queue.push(method);
}

void CCaller::call()
{
  if(!m_queue.empty())
  {
    script()->callMethod("void " + m_queue.front() + "()");
    m_queue.pop();
  }
}

}
}
