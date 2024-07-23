#pragma once
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace ha
{
namespace datetime
{

class CTimer
{
public:
  explicit CTimer(const std::string &scriptName, const std::string &functionName, const std::chrono::milliseconds &interval);
  virtual ~CTimer();

  void start();
  void stop ();
  void reset();

  const std::string &scriptName() const { return m_scriptName; }
  const std::string &functionName() const { return m_functionName; }

  CTimer &operator=(const CTimer &other);

protected:
  virtual bool triggered() = 0;

private:
  std::string m_scriptName;
  std::string m_functionName;
  std::chrono::milliseconds m_interval;
  std::chrono::steady_clock::time_point m_startTime;
  bool m_running;
  std::thread m_thread;
  std::mutex m_mutex;
  std::condition_variable m_cv;

  void run();
  void finishThread();
};

}
}
