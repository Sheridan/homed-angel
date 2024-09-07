#pragma once
#include <queue>
#include <string>
#include <thread>
#include "scripting/script/cscriptenvironment.h"
#include "homed/model/entities/cproperty.h"
#include "datetime/astronomical/csuntracker.h"
#include "mqtt/scripting/cmqttscriptcallback.h"
#include "log.h"
namespace ha
{
namespace scripting
{

#define HA_CALLER_SINGLE_METHOD_DEFINITION(_type) \
  void callMethod(const std::string &method, const std::string &params, _type value);

#define HA_CALLER_DOUBLE_METHOD_DEFINITION(_type_first,_type_second) \
  void callMethod(const std::string &method, const std::string &params, _type_first first, _type_second second);

class CCaller;
template<typename T> class CCallerSingle;
class CScript : public CScriptEnvironment
{
  using UPropertyCaller   = CCallerSingle<ha::homed::CProperty         *>;
  using USunTrackerCaller = CCallerSingle<ha::datetime::ESunTrackerEvent>;
  using UMqttCaller       = CCallerSingle<ha::mqtt::SMqttMesssage       >;
public:
  explicit CScript(const std::filesystem::path &file);
  virtual ~CScript();

  CScript(const CScript&) = delete;
  CScript& operator=(const CScript&) = delete;

  void run();

  CCaller           *simpleCaller    () { return m_simpleCaller    ; }
  UPropertyCaller   *propertyCaller  () { return m_propertyCaller  ; }
  USunTrackerCaller *sunTrackerCaller() { return m_sunTrackerCaller; }
  UMqttCaller       *mqttCaller      () { return m_mqttCaller      ; }

  void callMethod(const std::string &method, const std::string &params);
  HA_CALLER_SINGLE_METHOD_DEFINITION(ha::homed::CProperty          *);
  HA_CALLER_SINGLE_METHOD_DEFINITION(ha::datetime::ESunTrackerEvent );
  HA_CALLER_SINGLE_METHOD_DEFINITION(ha::mqtt::SMqttMesssage       &);


private:
  bool m_running;
  std::thread m_thread;
  CCaller           *m_simpleCaller    ;
  UPropertyCaller   *m_propertyCaller  ;
  USunTrackerCaller *m_sunTrackerCaller;
  UMqttCaller       *m_mqttCaller      ;

};




}
}
