#include "datetime/timers/ctimer.h"
#include "ctimer.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CTimer::CTimer(const std::string &scriptName, const std::string &functionName, const std::chrono::milliseconds &interval)
    : m_scriptName(scriptName),
      m_functionName(functionName),
      m_interval(interval),
      m_running(false)
{}

CTimer::~CTimer()
{
  HA_LOG_NFO("Desctructing "<< m_scriptName << ":" << m_functionName << " timer");
  stop();
}

void CTimer::start()
{
  if (m_running)
  {
    reset();
  }
  else
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_running = true;
    m_thread = std::thread(&CTimer::run, this);
  }
}

void CTimer::stop()
{
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_running = false;
  }
  m_cv.notify_all();
  if (m_thread.joinable())
  {
    m_thread.join();
  }
}

void CTimer::reset()
{
  HA_LOG_NFO("Reseting "<< m_scriptName << ":" << m_functionName << " timer");
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_startTime = std::chrono::steady_clock::now();
  }
  m_cv.notify_all();
}

}
}
