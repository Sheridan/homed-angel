#include "datetime/astronomical/castronomical.h"
#include "st.h"
#include "castronomical.h"

namespace ha
{
namespace datetime
{


CAstronomical::CAstronomical()
{
  m_sunTracker = new CSunTracker();

}

CAstronomical::~CAstronomical()
{
  stop();
  delete m_sunTracker;
}

void CAstronomical::start()
{
  m_thread = std::thread(&CAstronomical::run, this);
}

void CAstronomical::stop()
{
  m_running = false;
  if (m_thread.joinable())
  {
    m_thread.join();
  }
}

void CAstronomical::run()
{
  while (m_running)
  {
    m_sunTracker->update();
    HA_ST.sleep();
  }
}
}
}
