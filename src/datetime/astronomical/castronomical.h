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
  explicit CAstronomical();
  virtual ~CAstronomical();

  CSunTracker *sun();
  void start();
  void stop();

private:
  std::thread m_updateThread;
  std::thread m_checkThread;
  std::atomic<bool> m_running;
  CSunTracker *m_sunTracker;

  void runUpdate();
  void runCheck();
};


}
}
