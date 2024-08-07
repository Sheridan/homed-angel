#include "datetime/astronomical/castronomical.h"
#include "utils/thread.h"
#include "st.h"

namespace ha
{
namespace datetime
{

CAstronomical::CAstronomical()
  : m_sunTracker(nullptr)
{
  m_sunTracker = new CSunTracker();
}

CAstronomical::~CAstronomical()
{
  stop();
  delete m_sunTracker;
}

CSunTracker *CAstronomical::sun()
{
  return m_sunTracker;
};

void CAstronomical::start()
{
  m_running = true;
  m_updateThread = std::thread(&CAstronomical::runUpdate, this);
  m_checkThread  = std::thread(&CAstronomical::runCheck, this);
}

void CAstronomical::stop()
{
  m_running = false;
  if (m_checkThread .joinable()) { m_checkThread .join(); }
  if (m_updateThread.joinable()) { m_updateThread.join(); }
}

void CAstronomical::runUpdate()
{
  ha::utils::set_thread_name("Astro update");
  while (m_running)
  {
    for (short i = 0; i < 60; i++)
    {
      ha::utils::sleep(1000);
      if(!m_running) { return; }
    }
    m_sunTracker->update();
  }
}

void CAstronomical::runCheck()
{
  ha::utils::set_thread_name("Astro check");
  while (m_running)
  {
    m_sunTracker->check();
    ha::utils::sleep();
  }
}

}
}
