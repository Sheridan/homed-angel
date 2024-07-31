#include "datetime/timers/ctimeroneshoot.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CTimerOneshot::CTimerOneshot(const std::string& scriptName, const std::string &functionName, const std::chrono::milliseconds &interval)
  : CTimer(scriptName, functionName), ha::scripting::CScriptObject()
{
  setInterval(interval);
}

CTimerOneshot::~CTimerOneshot()
{}

std::chrono::milliseconds CTimerOneshot::triggered()
{
  HA_ST->angel()->manager()->script(scriptName())->queueSimpleFunctionCall(functionName());
  HA_LOG_VERBOSE("Oneshot timer [" << scriptName() << ":" << functionName() << "] triggered");
  return std::chrono::milliseconds(0);
}

}
}
