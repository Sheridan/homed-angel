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
{
  HA_LOG_NFO("Constructing "<< m_scriptName << ":" << m_functionName << " timer");
}

void CTimer::finishThread()
{
  if (m_thread.joinable())
  {
    m_thread.join();
  }
}

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
    finishThread();
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
  finishThread();
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

void CTimer::run()
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
      if(triggered())
      {
        m_running = false;
        break;
      }
      else
      {
        m_startTime = std::chrono::steady_clock::now();
        remaining = m_interval;
      }
    }

    if (m_cv.wait_for(lock, remaining, [this] { return !m_running; }))
    {
      break;
    }
  }
}

CTimer &CTimer::operator=(const CTimer &other)
{
  m_scriptName = other.m_scriptName;
  m_functionName = other.m_functionName;
  m_interval = other.m_interval;
  return *this;
}


}
}
