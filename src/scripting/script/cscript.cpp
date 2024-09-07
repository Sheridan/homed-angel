#include "scripting/script/cscript.h"
#include "scripting/script/callers/ccaller.h"
#include "scripting/script/callers/ccallersingle.h"
#include "utils/thread.h"
#include "st.h"

namespace ha
{
namespace scripting
{

#define HA_CALLER_SINGLE_METHOD(_type) \
  void CScript::callMethod(const std::string &method, const std::string &params, _type param)

#define HA_CALLER_DOUBLE_METHOD(_type_first,_type_second) \
  void CScript::callMethod(const std::string &method, const std::string &params, _type_first first, _type_second second)

#define HA_CALLER_METHOD_BODY(_argset) \
  { \
    std::string fullMethod = "void " + method + params; \
    HA_LOG_DBG_SCRIPT("Calling method '" << fullMethod << "' from " << name()); \
    asIScriptFunction *function = m_builder->GetModule()->GetFunctionByDecl(fullMethod.c_str()); \
    if(function) \
    { \
      HA_AS_ACCERT_CALL(m_context->Prepare(function)); \
      _argset; \
      if(m_context->Execute() != asEXECUTION_FINISHED) \
      { \
        HA_LOG_ERR("Calling method '" << fullMethod << "' from " << name() << " failed"); \
      } \
    } \
    else \
    { \
      HA_LOG_ERR("Calling method '" << fullMethod << "': can not get function"); \
    } \
  }

void CScript::callMethod(const std::string &method, const std::string &params)
                                                           HA_CALLER_METHOD_BODY(                                                        );
HA_CALLER_SINGLE_METHOD(ha::homed::CProperty          *)   HA_CALLER_METHOD_BODY(m_context->SetArgObject(0,                       param) );
HA_CALLER_SINGLE_METHOD(ha::mqtt::SMqttMesssage       &)   HA_CALLER_METHOD_BODY(m_context->SetArgObject(0,                      &param) );
HA_CALLER_SINGLE_METHOD(ha::datetime::ESunTrackerEvent )   HA_CALLER_METHOD_BODY(m_context->SetArgDWord (0,  static_cast<asDWORD>(param)));

CScript::CScript(const std::filesystem::path &file)
  : CScriptEnvironment(file),
    m_running(false),
    m_thread(&CScript::run, this)
{
  m_simpleCaller     = new CCaller          (this                            );
  m_propertyCaller   = new UPropertyCaller  (this, "(CProperty @property)"   );
  m_sunTrackerCaller = new USunTrackerCaller(this, "(ESunTrackerEvent event)");
  m_mqttCaller       = new UMqttCaller      (this, "(SMqttMesssage &in msg)" );
}

CScript::~CScript()
{
  HA_ST->homed()->unsubscribeScript(name());
  HA_ST->astro()->sun()->unsubscribe(name());
  HA_ST->mqtt()->unsubscribe(name());
  delete m_simpleCaller;
  delete m_propertyCaller;
  delete m_sunTrackerCaller;
  delete m_mqttCaller;
  if (m_thread.joinable())
  {
    HA_LOG_VERBOSE("Releasing thread " << m_thread.get_id() << " for " << file().string());
    m_running = false;
    m_thread.join();
  }
}

void CScript::run()
{
  HA_LOG_VERBOSE("Strating thread " << m_thread.get_id() << " for " << file().string());
  ha::utils::set_thread_name(name());
  m_running = true;
  registerEntities();
  if(build())
  {
    callMethod("initialize", "()");
    while (m_running)
    {
      m_simpleCaller    ->call();
      m_propertyCaller  ->call();
      m_sunTrackerCaller->call();
      m_mqttCaller      ->call();
      ha::utils::sleep(10);
    }
    callMethod("deinitialize", "()");
  }
}

}
}
