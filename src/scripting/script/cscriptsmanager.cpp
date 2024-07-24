#include "scripting/script/cscriptsmanager.h"
#include "st.h"

namespace ha
{
namespace scripting
{

CScriptsManager::CScriptsManager() {}

CScriptsManager::~CScriptsManager()
{
  for(CScript *script : m_scripts)
  {
    delete script;
  }
  m_scripts.clear();
}

void CScriptsManager::attachScript(const std::filesystem::path &file)
{
  if (!std::filesystem::exists(file)) { HA_ABORT(file.string() << " is not exists o0"); }
  const CScript *spt = script(file);
  if(spt)
  {
    detachScript(file);
  }

  m_scripts.push_back(new CScript(file));
}

void CScriptsManager::detachScript(const std::filesystem::path &file)
{
  for (auto it = m_scripts.begin(); it != m_scripts.end();)
  {
    CScript *script = *it;
    if(script->file() == file)
    {
      delete script;
      it = m_scripts.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

CScript *CScriptsManager::script(const std::filesystem::path &file)
{
  for(CScript *script : m_scripts)
  {
    if(script->file() == file) { return script; }
  }
  HA_LOG_DBG_SCRIPT("Script manager can not found " << file.string());
  return nullptr;
}

CScript *CScriptsManager::script(const std::string &name)
{
  for(CScript *script : m_scripts)
  {
    if(script->name() == name) { return script; }
  }
  HA_LOG_DBG_SCRIPT("Script manager can not found " << name);
  return nullptr;
}

}
}
