#pragma once
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "datetime/entities/ctimeinterval.h"
#include "datetime/entities/cdatetime.h"

namespace ha
{
namespace datetime
{

class CTimer
{
public:
  explicit CTimer(const std::string &scriptName, const std::string &functionName);
  virtual ~CTimer();

  void start();
  void stop ();
  void reset();

  const std::string &scriptName() const { return m_scriptName; }
  const std::string &functionName() const { return m_functionName; }
  const CTimeInterval &timeInterval() const { return m_TInterval; }
  const CDateTime     &nextTrigger() const { return m_nextTrigger; }

  CTimer &operator=(const CTimer &other);

protected:
  virtual std::chrono::milliseconds triggered() = 0;
  void setInterval(const std::chrono::milliseconds &interval);

private:
  std::string m_scriptName;
  std::string m_functionName;
  std::chrono::milliseconds m_interval;
  std::chrono::steady_clock::time_point m_startTime;
  bool m_running;
  std::thread m_thread;
  std::mutex m_mutex;
  std::condition_variable m_cv;

  CTimeInterval m_TInterval;
  CDateTime     m_nextTrigger;

  void run();
  void finishThread();
};

}
}
