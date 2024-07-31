#pragma once
#include "datetime/timers/ctimer.h"
#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace datetime
{

class CTimerCron : public CTimer, public ha::scripting::CScriptObject
{
public:
  explicit CTimerCron(const std::string &scriptName, const std::string &functionName, const std::string& schedule);
  virtual ~CTimerCron();

private:
  std::string m_schedule;
  std::chrono::milliseconds triggered() final;
  std::chrono::milliseconds nextInterval();
  std::chrono::milliseconds convertToInterval(const std::chrono::system_clock::time_point &time, const std::chrono::system_clock::time_point &now);
};

}
}
