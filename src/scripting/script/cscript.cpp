#include "scripting/script/cscript.h"
#include "st.h"

namespace ha
{
namespace scripting
{

CScript::CScript(const std::filesystem::path &file)
  : CScriptEnvironment(file),
    m_running(false),
    m_thread(&CScript::run, this)
{}

CScript::~CScript()
{
  HA_ST->homed()->unsubscribeScript(name());
  // HA_ST->timers().unsubscribeScript(name());
  if (m_thread.joinable())
  {
    HA_LOG_NFO("Releasing thread " << m_thread.get_id() << " for " << file().string());
    m_running = false;
    m_thread.join();
  }
}

void CScript::callMethod(const std::string &method)
{
  asIScriptFunction *function = m_builder->GetModule()->GetFunctionByDecl(method.c_str());
  if(function)
  {
    HA_AS_ACCERT_CALL(m_context->Prepare(function));
    if(m_context->Execute() != asEXECUTION_FINISHED)
    {
      HA_LOG_ERR("Calling method '" << method << "' from " << name() << " failed");
    }
  }
  else
  {
    HA_LOG_ERR("Can not found method '" << method << "' in " << name());
  }
}

void CScript::run()
{
  HA_LOG_NFO("Strating thread " << m_thread.get_id() << " for " << file().string());
  m_running = true;
  registerEntities();
  if(build())
  {
    callMethod("void initialize()");
    while (m_running)
    {
      if(!m_timerShoots.empty())
      {
        callTimerShoot(m_timerShoots.front());
        m_timerShoots.pop();
      }
      if(!m_propertyUpdates.empty())
      {
        callPropertyChanged(m_propertyUpdates.front());
        m_propertyUpdates.pop();
      }
      HA_ST->sleep();
    }
    callMethod("void deinitialize()");
  }
}

void CScript::queuePropertyChanged(const std::string &method, ha::homed::CProperty *property)
{
  m_propertyUpdates.push(SPropertyUpdate(method, property));
}

void CScript::queueSimpleFunctionCall(const std::string &method)
{
  m_timerShoots.push(STimerShoot(method));
}

void CScript::callPropertyChanged(const SPropertyUpdate &propertyUpdate)
{
  std::string method = "void " + propertyUpdate.method + "(CProperty @property)";
  HA_LOG_DBG_SCRIPT("Calling method '" << method << "' from " << name());
  asIScriptFunction *function = m_builder->GetModule()->GetFunctionByDecl(method.c_str());
  if(function)
  {
    HA_AS_ACCERT_CALL(m_context->Prepare(function));
    m_context->SetArgObject(0, propertyUpdate.property);
    if(m_context->Execute() != asEXECUTION_FINISHED)
    {
      HA_LOG_ERR("Calling method '" << method << "' from " << name() << " failed");
    }
    // m_context->Release();
  }
  else
  {
    HA_LOG_ERR("Calling method '" << method << "': can not get function");
  }
}

void CScript::callTimerShoot(const STimerShoot &timerShoot)
{
  callMethod("void " + timerShoot.method + "()");
}

}
}
