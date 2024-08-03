#pragma once
#include <queue>
#include <string>
#include <thread>
#include "scripting/script/cscriptenvironment.h"
#include "homed/model/entities/cproperty.h"
#include "datetime/astronomical/csuntracker.h"
#include "log.h"
namespace ha
{
namespace scripting
{

#define HA_CALL_CALLER_METHOD(_tparam, _argset) \
  template <typename T> void callMethod(const std::string &method, const std::string &params, T _tparam param) \
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

class CCaller;
template<typename T> class CCallerSingle;
class CScript : public CScriptEnvironment
{
  using UPropertyCaller = CCallerSingle<ha::homed::CProperty *>;
  using USunTrackerCaller = CCallerSingle<ha::datetime::ESunTrackerEvent>;
public:
  explicit CScript(const std::filesystem::path &file);
  virtual ~CScript();

  CScript(const CScript&) = delete;
  CScript& operator=(const CScript&) = delete;

  void run();

  CCaller *          simpleCaller  () { return m_simpleCaller  ; };
  UPropertyCaller   *propertyCaller() { return m_propertyCaller; };
  USunTrackerCaller *sunTrackerCaller() { return m_sunTrackerCaller; };

  void callMethod(const std::string &method);


  HA_CALL_CALLER_METHOD( *, m_context->SetArgObject(0, param));
  HA_CALL_CALLER_METHOD(  , m_context->SetArgDWord(0, static_cast<asDWORD>(param)));

private:
  bool m_running;
  std::thread m_thread;
  CCaller *m_simpleCaller;
  UPropertyCaller *m_propertyCaller;
  USunTrackerCaller *m_sunTrackerCaller;

};




}
}
