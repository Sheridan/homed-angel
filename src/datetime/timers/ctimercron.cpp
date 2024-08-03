// https://github.com/mariusbancila/croncpp
#include "scripting/script/callers/ccaller.h"

#include "datetime/timers/ctimercron.h"
#include "croncpp.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CTimerCron::CTimerCron(const std::string &scriptName, const std::string &functionName, const std::string &schedule)
  : CTimer(scriptName, functionName), ha::scripting::CScriptObject(),
    m_schedule(schedule)
{
  setInterval(nextInterval());
}

CTimerCron::~CTimerCron()
{}

std::chrono::milliseconds CTimerCron::triggered()
{
  HA_ST->angel()->manager()->script(scriptName())->simpleCaller()->append(functionName());
  HA_LOG_VERBOSE("Cron timer [" << scriptName() << ":" << functionName() << "] triggered.");
  return nextInterval();
}

std::chrono::milliseconds CTimerCron::nextInterval()
{
  try
  {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return convertToInterval(cron::cron_next<cron::cron_oracle_traits>(cron::make_cron<cron::cron_oracle_traits>(m_schedule), now), now - std::chrono::milliseconds(500));
  }
  catch (cron::bad_cronexpr const & ex)
  {
    HA_LOG_ERR("Bad cron expression: " << ex.what());
  }
  return std::chrono::milliseconds(0);
}

std::chrono::milliseconds CTimerCron::convertToInterval(const std::chrono::system_clock::time_point &time, const std::chrono::system_clock::time_point &now)
{
  #ifdef HA_TIMER_DEBUG
  std::time_t time_t_value = std::chrono::system_clock::to_time_t(time);
  HA_LOG_DBG_TIMER("Cron next: " << std::put_time(std::localtime(&time_t_value), "%Y.%m.%d %H:%M:%S"));
  #endif
  return std::chrono::duration_cast<std::chrono::milliseconds>(time - now);
}
}
}
