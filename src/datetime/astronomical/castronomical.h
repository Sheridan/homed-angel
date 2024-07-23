#pragma once
#include <thread>
#include <atomic>
#include "datetime/astronomical/csuntracker.h"

namespace ha
{
namespace datetime
{

class CAstronomical
{
public:
  CAstronomical();
  ~CAstronomical();

  CSunTracker *sun() { return m_sunTracker; };
  void start();
  void stop();

private:
  std::thread m_thread;
  std::atomic<bool> m_running;
  CSunTracker *m_sunTracker;

  void run();
};


}
}
