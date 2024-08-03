#include "scripting/script/cscript.h"
#include "scripting/script/callers/ccaller.h"
#include "scripting/script/callers/ccallersingle.h"
#include "st.h"

namespace ha
{
namespace scripting
{

CScript::CScript(const std::filesystem::path &file)
  : CScriptEnvironment(file),
    m_running(false),
    m_thread(&CScript::run, this)
{
  m_simpleCaller = new CCaller(this);
  m_propertyCaller = new UPropertyCaller(this, "(CProperty @property)");
  m_sunTrackerCaller = new USunTrackerCaller(this, "(ESunTrackerEvent event)");
}

CScript::~CScript()
{
  HA_ST->homed()->unsubscribeScript(name());
  HA_ST->astro()->sun()->unsubscribe(name());
  // HA_ST->timers().unsubscribeScript(name());
  delete m_simpleCaller;
  delete m_propertyCaller;
  delete m_sunTrackerCaller;
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
      m_simpleCaller->call();
      m_propertyCaller->call();
      HA_ST->sleep(10);
    }
    callMethod("void deinitialize()");
  }
}

}
}
