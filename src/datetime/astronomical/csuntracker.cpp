#include "datetime/astronomical/csuntracker.h"
#include "scripting/script/callers/ccallersingle.h"
#include <iostream>
#include <libnova/solar.h>
#include <libnova/julian_day.h>
#include <libnova/rise_set.h>
#include <libnova/transform.h>
#include <chrono>
#include <ctime>
#include "st.h"


#define HA_SUN_ANGULAR_SIZE     0.5
#define HA_HORIZON_ZERO         0.0
#define HA_HORIZON_STANDART     LN_SOLAR_STANDART_HORIZON
#define HA_HORIZON_CIVIL        LN_SOLAR_CIVIL_HORIZON
#define HA_HORIZON_NAUTIC       LN_SOLAR_NAUTIC_HORIZON
#define HA_HORIZON_ASTRONOMICAL LN_SOLAR_ASTRONOMICAL_HORIZON

namespace ha
{
namespace datetime
{

CSunTracker::CSunTracker()
  : CTracker(),
    m_circumpolar(false)
{
  HA_LOG_DBG_ASTRO("CSunTracker init");
  HA_LOG_DBG_ASTRO("CSunTracker " << m_observer.lat << ":" << m_observer.lng);
  update();
  #ifdef HA_ASTRO_DEBUG
  printEvents();
  #endif
}

CSunTracker::~CSunTracker()
{
  HA_LOG_DBG_ASTRO("CSunTracker destroy");
  for(auto &pair : m_eventsTime)
  {
    delete pair.second;
  }
  m_eventsTime.clear();
}

#ifdef HA_ASTRO_DEBUG
#define HA_PRINT_DATE(_type) HA_LOG_NFO(getEventTime(ESunTrackerEvent::ste##_type)->asString("%Y.%m.%d %H:%M:%S") << " | " << #_type);
void CSunTracker::printEvents()
{

  HA_PRINT_DATE(Nadir);
  HA_PRINT_DATE(MorningBlueHourStart);
  HA_PRINT_DATE(MorningAstronomicalTwilightStart);
  HA_PRINT_DATE(MorningAstronomicalTwilightEnd);
  HA_PRINT_DATE(MorningNauticalTwilightStart);
  HA_PRINT_DATE(MorningNauticalTwilightEnd);
  HA_PRINT_DATE(MorningCivilTwilightStart);
  HA_PRINT_DATE(MorningCivilTwilightEnd);
  HA_PRINT_DATE(MorningBlueHourEnd);
  HA_PRINT_DATE(MorningGoldenHourStart);
  HA_PRINT_DATE(MorningSunriseStart);
  HA_PRINT_DATE(MorningSunriseEnd);
  HA_PRINT_DATE(MorningGoldenHourEnd);
  HA_PRINT_DATE(Zenith);
  HA_PRINT_DATE(EveningGoldenHourStart);
  HA_PRINT_DATE(EveningGoldenHourEnd);
  HA_PRINT_DATE(EveningBlueHourStart);
  HA_PRINT_DATE(EveningSunsetStart);
  HA_PRINT_DATE(EveningSunsetEnd);
  HA_PRINT_DATE(EveningBlueHourEnd);
  HA_PRINT_DATE(EveningCivilTwilightStart);
  HA_PRINT_DATE(EveningCivilTwilightEnd);
  HA_PRINT_DATE(EveningNauticalTwilightStart);
  HA_PRINT_DATE(EveningNauticalTwilightEnd);
  HA_PRINT_DATE(EveningAstronomicalTwilightStart);
  HA_PRINT_DATE(EveningAstronomicalTwilightEnd);

}
#endif

void CSunTracker::update()
{
  if(!circumpolar())
  {
    HA_LOG_DBG_ASTRO("CSunTracker update");
    update(ESunTrackerEvent::steNadir                           );
    update(ESunTrackerEvent::steMorningBlueHourStart            );
    update(ESunTrackerEvent::steMorningAstronomicalTwilightStart);
    update(ESunTrackerEvent::steMorningAstronomicalTwilightEnd  );
    update(ESunTrackerEvent::steMorningNauticalTwilightStart    );
    update(ESunTrackerEvent::steMorningNauticalTwilightEnd      );
    update(ESunTrackerEvent::steMorningCivilTwilightStart       );
    update(ESunTrackerEvent::steMorningCivilTwilightEnd         );
    update(ESunTrackerEvent::steMorningBlueHourEnd              );
    update(ESunTrackerEvent::steMorningGoldenHourStart          );
    update(ESunTrackerEvent::steMorningSunriseStart             );
    update(ESunTrackerEvent::steMorningSunriseEnd               );
    update(ESunTrackerEvent::steMorningGoldenHourEnd            );
    update(ESunTrackerEvent::steZenith                          );
    update(ESunTrackerEvent::steEveningGoldenHourStart          );
    update(ESunTrackerEvent::steEveningGoldenHourEnd            );
    update(ESunTrackerEvent::steEveningBlueHourStart            );
    update(ESunTrackerEvent::steEveningSunsetStart              );
    update(ESunTrackerEvent::steEveningSunsetEnd                );
    update(ESunTrackerEvent::steEveningBlueHourEnd              );
    update(ESunTrackerEvent::steEveningCivilTwilightStart       );
    update(ESunTrackerEvent::steEveningCivilTwilightEnd         );
    update(ESunTrackerEvent::steEveningNauticalTwilightStart    );
    update(ESunTrackerEvent::steEveningNauticalTwilightEnd      );
    update(ESunTrackerEvent::steEveningAstronomicalTwilightStart);
    update(ESunTrackerEvent::steEveningAstronomicalTwilightEnd  );
  }
}

void CSunTracker::check()
{
  if(!circumpolar())
  {
    HA_LOG_DBG_ASTRO("CSunTracker check");
    check(ESunTrackerEvent::steNadir                           );
    check(ESunTrackerEvent::steMorningBlueHourStart            );
    check(ESunTrackerEvent::steMorningAstronomicalTwilightStart);
    check(ESunTrackerEvent::steMorningAstronomicalTwilightEnd  );
    check(ESunTrackerEvent::steMorningNauticalTwilightStart    );
    check(ESunTrackerEvent::steMorningNauticalTwilightEnd      );
    check(ESunTrackerEvent::steMorningCivilTwilightStart       );
    check(ESunTrackerEvent::steMorningCivilTwilightEnd         );
    check(ESunTrackerEvent::steMorningBlueHourEnd              );
    check(ESunTrackerEvent::steMorningGoldenHourStart          );
    check(ESunTrackerEvent::steMorningSunriseStart             );
    check(ESunTrackerEvent::steMorningSunriseEnd               );
    check(ESunTrackerEvent::steMorningGoldenHourEnd            );
    check(ESunTrackerEvent::steZenith                          );
    check(ESunTrackerEvent::steEveningGoldenHourStart          );
    check(ESunTrackerEvent::steEveningGoldenHourEnd            );
    check(ESunTrackerEvent::steEveningBlueHourStart            );
    check(ESunTrackerEvent::steEveningSunsetStart              );
    check(ESunTrackerEvent::steEveningSunsetEnd                );
    check(ESunTrackerEvent::steEveningBlueHourEnd              );
    check(ESunTrackerEvent::steEveningCivilTwilightStart       );
    check(ESunTrackerEvent::steEveningCivilTwilightEnd         );
    check(ESunTrackerEvent::steEveningNauticalTwilightStart    );
    check(ESunTrackerEvent::steEveningNauticalTwilightEnd      );
    check(ESunTrackerEvent::steEveningAstronomicalTwilightStart);
    check(ESunTrackerEvent::steEveningAstronomicalTwilightEnd  );
  }
}

void CSunTracker::check(const ESunTrackerEvent &event)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if(m_eventsTime.contains(event))
  {
    if(thisTimeIsEvent(m_eventsTime[event]))
    {
      for(const SSunTrackerItem &item : m_events)
      {
        if(item.event == event)
        {
          HA_ST->angel()->manager()->script(item.scriptName)->sunTrackerCaller()->append(item.functionName, event);
        }
      }
    }
  }
}

void CSunTracker::update(const ESunTrackerEvent &event)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if(m_eventsTime.contains(event))
  { delete m_eventsTime[event]; }
  m_eventsTime[event] = new CDateTime(zonedateToChrono(calcEventTime(event)));
}

ln_zonedate CSunTracker::calcEventTime(ESunTrackerEvent event)
{
  double jd = julian();
  switch (event)
  {
    case ESunTrackerEvent::steNadir                           : { return calcTimes(jd, HA_HORIZON_ZERO                              ).nadir ; }; break;
    case ESunTrackerEvent::steMorningBlueHourStart            :
    case ESunTrackerEvent::steMorningAstronomicalTwilightStart: { return calcTimes(jd, HA_HORIZON_ASTRONOMICAL                      ).rise  ; }; break;
    case ESunTrackerEvent::steMorningAstronomicalTwilightEnd  :
    case ESunTrackerEvent::steMorningNauticalTwilightStart    : { return calcTimes(jd, HA_HORIZON_NAUTIC                            ).rise  ; }; break;
    case ESunTrackerEvent::steMorningNauticalTwilightEnd      :
    case ESunTrackerEvent::steMorningCivilTwilightStart       : { return calcTimes(jd, HA_HORIZON_CIVIL                             ).rise  ; }; break;
    case ESunTrackerEvent::steMorningCivilTwilightEnd         :
    case ESunTrackerEvent::steMorningBlueHourEnd              :
    case ESunTrackerEvent::steMorningGoldenHourStart          :
    case ESunTrackerEvent::steMorningSunriseStart             : { return calcTimes(jd, HA_HORIZON_STANDART                          ).rise  ; }; break;
    case ESunTrackerEvent::steMorningSunriseEnd               : { return calcTimes(jd, HA_HORIZON_STANDART + HA_SUN_ANGULAR_SIZE    ).rise  ; }; break;
    case ESunTrackerEvent::steMorningGoldenHourEnd            : { return calcTimes(jd, -LN_SOLAR_CIVIL_HORIZON                      ).rise  ; }; break;
    case ESunTrackerEvent::steZenith                          : { return calcTimes(jd, HA_HORIZON_ZERO                              ).zenith; }; break;
    case ESunTrackerEvent::steEveningGoldenHourStart          : { return calcTimes(jd, -LN_SOLAR_CIVIL_HORIZON                      ).set   ; }; break;
    case ESunTrackerEvent::steEveningGoldenHourEnd            :
    case ESunTrackerEvent::steEveningBlueHourStart            :
    case ESunTrackerEvent::steEveningSunsetStart              : { return calcTimes(jd, HA_HORIZON_STANDART                          ).set   ; }; break;
    case ESunTrackerEvent::steEveningCivilTwilightStart       :
    case ESunTrackerEvent::steEveningSunsetEnd                : { return calcTimes(jd, HA_HORIZON_STANDART + HA_SUN_ANGULAR_SIZE    ).set   ; }; break;
    case ESunTrackerEvent::steEveningBlueHourEnd              :
    case ESunTrackerEvent::steEveningCivilTwilightEnd         : { return calcTimes(jd, HA_HORIZON_CIVIL                             ).set   ; }; break;
    case ESunTrackerEvent::steEveningNauticalTwilightStart    :
    case ESunTrackerEvent::steEveningAstronomicalTwilightStart:
    case ESunTrackerEvent::steEveningNauticalTwilightEnd      : { return calcTimes(jd, HA_HORIZON_NAUTIC                            ).set   ; }; break;
    case ESunTrackerEvent::steEveningAstronomicalTwilightEnd  : { return calcTimes(jd, HA_HORIZON_ASTRONOMICAL                      ).set   ; }; break;
  }
  return ln_zonedate();
}

void CSunTracker::subscribe(const std::string &scriptName, const std::string &functionName, const ESunTrackerEvent &event)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_ASTRO("CSunTracker subscribe " << scriptName << ":" << functionName << " " << event);
  m_events.push_back(SSunTrackerItem(event, scriptName, functionName));
}

void CSunTracker::unsubscribe(const std::string &scriptName)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_ASTRO("CSunTracker unsubscribe script: " << scriptName);
  m_events.erase(std::remove_if (
                                  m_events.begin(),
                                  m_events.end(),
                                  [&scriptName](const SSunTrackerItem& event)
                                  {
                                    return event.scriptName == scriptName;
                                  }
                                ),
                  m_events.end());
}

SSunTrackerTime CSunTracker::calcTimes(const double &jd, const double &horizon)
{
  SSunTrackerTime result;
  ln_rst_time rst;
  if (ln_get_solar_rst_horizon(jd, &m_observer, horizon, &rst) != 0)
  {
    HA_LOG_DBG_ASTRO("Sun is circumpolar");
    result.circumpolar = true;
  }
  else
  {
    ln_get_local_date(rst.rise         , &result.rise  );
    ln_get_local_date(rst.transit      , &result.zenith);
    ln_get_local_date(rst.set          , &result.set   );
    ln_get_local_date(rst.transit + 0.5, &result.nadir );
    result.circumpolar = false;
  }
  m_circumpolar = result.circumpolar;
  HA_LOG_DBG_ASTRO("Sun is circumpolar? " << m_circumpolar);
  return result;
}

CDateTime *CSunTracker::getEventTime(const ha::datetime::ESunTrackerEvent &event)
{
  if(m_eventsTime.contains(event)) { return m_eventsTime[event]; }
  return nullptr;
}

bool CSunTracker::circumpolar()
{
  return m_circumpolar;
}

}
}
