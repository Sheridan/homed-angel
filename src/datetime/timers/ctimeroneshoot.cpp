#include "datetime/timers/ctimeroneshoot.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CTimerOneshot::CTimerOneshot(const std::string& scriptName, const std::string &functionName, const std::chrono::milliseconds &interval)
  : CTimer(scriptName, functionName, interval), ha::scripting::CScriptObject()
{}

CTimerOneshot::~CTimerOneshot()
{}

bool CTimerOneshot::triggered()
{
  HA_ST.angel().manager()->script(scriptName())->queueSimpleFunctionCall(functionName());
  HA_LOG_VERBOSE("Oneshot timer [" << scriptName() << ":" << functionName() << "] triggered");
  return true;
}

}
}
