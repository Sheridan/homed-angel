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

void CTimerOneshot::run()
{
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_startTime = std::chrono::steady_clock::now();
  }
  std::unique_lock<std::mutex> lock(m_mutex);
  while (m_running)
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime);
    auto remaining = m_interval - elapsed;

    if (remaining <= std::chrono::milliseconds(0))
    {
      HA_ST.angel().manager()->script(m_scriptName)->queueTimerShoot(m_functionName);
      HA_LOG_VERBOSE("Oneshot timer [" << m_scriptName << ":" << m_functionName << "] triggered");
      m_running = false;
      break;
    }

    if (m_cv.wait_for(lock, remaining, [this] { return !m_running; }))
    {
      break;
    }
  }
}

CTimerOneshot &CTimerOneshot::operator=(const CTimerOneshot &other)
{
  m_scriptName = other.m_scriptName;
  m_functionName = other.m_functionName;
  m_interval = other.m_interval;
  return *this;
}

}
}
