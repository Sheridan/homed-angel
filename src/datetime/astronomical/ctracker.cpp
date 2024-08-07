#include "datetime/astronomical/ctracker.h"
#include <libnova/julian_day.h>
#include <ctime>
#include "utils/thread.h"
#include "st.h"


namespace ha
{
namespace datetime
{


CTracker::CTracker()
  : m_latitude(HA_ST->config()->latitude()),
    m_longitude(HA_ST->config()->longitude()),
    m_altitude(HA_ST->config()->altitude()),
    m_observer({m_latitude, m_longitude})
{
  HA_LOG_DBG("CTracker");
}

CTracker::~CTracker()
{}

double CTracker::julian()
{
  return ln_get_julian_from_sys();
}

std::chrono::steady_clock::time_point CTracker::zonedateToChrono(const ln_zonedate &zonedate)
{
  std::tm timeinfo = {};
  timeinfo.tm_year = zonedate.years - 1900;
  timeinfo.tm_mon = zonedate.months - 1;
  timeinfo.tm_mday = zonedate.days;
  timeinfo.tm_hour = zonedate.hours;
  timeinfo.tm_min = zonedate.minutes;
  timeinfo.tm_sec = static_cast<int>(zonedate.seconds);

  std::time_t time = std::mktime(&timeinfo);// + zonedate.gmtoff;

  return std::chrono::steady_clock::time_point(std::chrono::seconds(time));
}

bool CTracker::thisTimeIsEvent(const CDateTime &dt)
{
  int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                            std::chrono::system_clock::now()
                                                                            -
                                                                            std::chrono::system_clock::from_time_t(dt.asUnixTimestamp())
                                                                          ).count();
  return duration >= -(HA_DEFAULT_SLEEP_MS/2) && duration <= (HA_DEFAULT_SLEEP_MS/2);
}

}
}
