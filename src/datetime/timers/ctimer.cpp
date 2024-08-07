#include "datetime/timers/ctimer.h"
#include "utils/thread.h"
#include "st.h"


namespace ha
{
namespace datetime
{

CTimer::CTimer(const std::string &scriptName, const std::string &functionName)
    : m_scriptName(scriptName),
      m_functionName(functionName),
      m_interval(0),
      m_running(false),
      m_TInterval(0)
{
  HA_LOG_DBG_TIMER("Constructing "<< m_scriptName << ":" << m_functionName << " timer");
}

CTimer::~CTimer()
{
  HA_LOG_DBG_TIMER("Desctructing "<< m_scriptName << ":" << m_functionName << " timer");
  stop();
}

void CTimer::finishThread()
{
  if (m_thread.joinable())
  {
    m_thread.join();
  }
}

void CTimer::start()
{
  if(m_interval.count() > 0)
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
      setInterval(m_interval);
      m_thread = std::thread(&CTimer::run, this);
    }
  }
  else
  {
    HA_LOG_WRN("The timer interval is not set");
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
  // ha::utils::set_thread_name("Timer " + m_scriptName + ":" + m_functionName);
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
      std::chrono::milliseconds nextInterval = triggered();
      if(nextInterval.count() == 0)
      {
        m_running = false;
        break;
      }
      else
      {
        m_startTime = std::chrono::steady_clock::now();
        remaining = nextInterval;
        setInterval(nextInterval);
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

void CTimer::setInterval(const std::chrono::milliseconds &interval)
{
  m_interval = interval;
  m_TInterval = CTimeInterval(m_interval / 1000);
  m_nextTrigger = CDateTime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + interval));
}


}
}
