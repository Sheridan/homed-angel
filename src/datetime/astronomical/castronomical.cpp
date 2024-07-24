#include "datetime/astronomical/castronomical.h"
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
  while (m_running)
  {
    m_sunTracker->update();
    HA_ST->sleep(60 * 1000);
  }
}

void CAstronomical::runCheck()
{
  while (m_running)
  {
    m_sunTracker->check();
    HA_ST->sleep();
  }
}

}
}
