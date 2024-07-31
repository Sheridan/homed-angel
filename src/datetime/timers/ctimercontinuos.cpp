#include "datetime/timers/ctimercontinuos.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CTimerContinuous::CTimerContinuous(const std::string& scriptName, const std::string &functionName, const std::chrono::milliseconds &interval)
  : CTimer(scriptName, functionName), ha::scripting::CScriptObject(),
    m_interval(interval)
{
  setInterval(m_interval);
}

CTimerContinuous::~CTimerContinuous()
{}

std::chrono::milliseconds CTimerContinuous::triggered()
{
  HA_ST->angel()->manager()->script(scriptName())->queueSimpleFunctionCall(functionName());
  HA_LOG_VERBOSE("Continuous timer [" << scriptName() << ":" << functionName() << "] triggered.");
  return m_interval;
}

}
}
