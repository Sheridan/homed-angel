#pragma once
#include "datetime/timers/ctimer.h"
#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace datetime
{

class CTimerContinuous : public CTimer, public ha::scripting::CScriptObject
{
public:
  explicit CTimerContinuous(const std::string &scriptName, const std::string &functionName, const std::chrono::milliseconds &interval);
  virtual ~CTimerContinuous();

private:
  bool triggered() final;
};

}
}
