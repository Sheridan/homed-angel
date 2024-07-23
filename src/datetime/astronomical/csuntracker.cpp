#include "datetime/astronomical/csuntracker.h"
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
  : CTracker()
{
  #ifdef HA_DEBUG
  printEvents();
  #endif
}

CSunTracker::~CSunTracker()
{}

#ifdef HA_DEBUG
#define HA_PRINT_DATE(_type) HA_LOG_NFO(calcTime(ESunTrackerEvent::ste##_type).asString("%Y.%m.%d %H:%M:%S") << " | " << #_type);
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
  if(!calcTimes(julian(), HA_HORIZON_ZERO).circumpolar)
  {
    checkEvent(ESunTrackerEvent::steNadir                           );
    checkEvent(ESunTrackerEvent::steMorningBlueHourStart            );
    checkEvent(ESunTrackerEvent::steMorningAstronomicalTwilightStart);
    checkEvent(ESunTrackerEvent::steMorningAstronomicalTwilightEnd  );
    checkEvent(ESunTrackerEvent::steMorningNauticalTwilightStart    );
    checkEvent(ESunTrackerEvent::steMorningNauticalTwilightEnd      );
    checkEvent(ESunTrackerEvent::steMorningCivilTwilightStart       );
    checkEvent(ESunTrackerEvent::steMorningCivilTwilightEnd         );
    checkEvent(ESunTrackerEvent::steMorningBlueHourEnd              );
    checkEvent(ESunTrackerEvent::steMorningGoldenHourStart          );
    checkEvent(ESunTrackerEvent::steMorningSunriseStart             );
    checkEvent(ESunTrackerEvent::steMorningSunriseEnd               );
    checkEvent(ESunTrackerEvent::steMorningGoldenHourEnd            );
    checkEvent(ESunTrackerEvent::steZenith                          );
    checkEvent(ESunTrackerEvent::steEveningGoldenHourStart          );
    checkEvent(ESunTrackerEvent::steEveningGoldenHourEnd            );
    checkEvent(ESunTrackerEvent::steEveningBlueHourStart            );
    checkEvent(ESunTrackerEvent::steEveningSunsetStart              );
    checkEvent(ESunTrackerEvent::steEveningSunsetEnd                );
    checkEvent(ESunTrackerEvent::steEveningBlueHourEnd              );
    checkEvent(ESunTrackerEvent::steEveningCivilTwilightStart       );
    checkEvent(ESunTrackerEvent::steEveningCivilTwilightEnd         );
    checkEvent(ESunTrackerEvent::steEveningNauticalTwilightStart    );
    checkEvent(ESunTrackerEvent::steEveningNauticalTwilightEnd      );
    checkEvent(ESunTrackerEvent::steEveningAstronomicalTwilightStart);
    checkEvent(ESunTrackerEvent::steEveningAstronomicalTwilightEnd  );
  }
}

void CSunTracker::checkEvent(const ESunTrackerEvent &event)
{
  if(thisTimeIsEvent(calcTime(event)))
  {
    for(const SSunTrackerItem &item : m_events)
    {
      if(item.event == event)
      {
        HA_ST.angel().manager()->script(item.scriptName)->queueSimpleFunctionCall(item.functionName);
      }
    }
  }
}

void CSunTracker::subscribe(const ESunTrackerEvent &event, const std::string &scriptName, const std::string &functionName)
{
  m_events.push_back(SSunTrackerItem(event, scriptName, functionName));
}

SSunTrackerTime CSunTracker::calcTimes(const double &jd, const double &horizon)
{
  SSunTrackerTime result;
  ln_rst_time rst;
  if (ln_get_solar_rst_horizon(jd, &m_observer, horizon, &rst) != 0)
  {
    HA_LOG_NFO("Sun is circumpolar");
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
  return result;
}

CDateTime CSunTracker::calcTime(const ESunTrackerEvent &event)
{
  double jd = julian();
  switch (event)
  {
    case ESunTrackerEvent::steNadir                           : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_ZERO                              ).nadir )); }; break;
    case ESunTrackerEvent::steMorningBlueHourStart            :
    case ESunTrackerEvent::steMorningAstronomicalTwilightStart: { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_ASTRONOMICAL                      ).rise  )); }; break;
    case ESunTrackerEvent::steMorningAstronomicalTwilightEnd  :
    case ESunTrackerEvent::steMorningNauticalTwilightStart    : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_NAUTIC                            ).rise  )); }; break;
    case ESunTrackerEvent::steMorningNauticalTwilightEnd      :
    case ESunTrackerEvent::steMorningCivilTwilightStart       : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_CIVIL                             ).rise  )); }; break;
    case ESunTrackerEvent::steMorningCivilTwilightEnd         :
    case ESunTrackerEvent::steMorningBlueHourEnd              :
    case ESunTrackerEvent::steMorningGoldenHourStart          :
    case ESunTrackerEvent::steMorningSunriseStart             : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_STANDART                          ).rise  )); }; break;
    case ESunTrackerEvent::steMorningSunriseEnd               : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_STANDART + HA_SUN_ANGULAR_SIZE    ).rise  )); }; break;
    case ESunTrackerEvent::steMorningGoldenHourEnd            : { return CDateTime(zonedateToChrono(calcTimes(jd, -LN_SOLAR_CIVIL_HORIZON                      ).rise  )); }; break;
    case ESunTrackerEvent::steZenith                          : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_ZERO                              ).zenith)); }; break;
    case ESunTrackerEvent::steEveningGoldenHourStart          : { return CDateTime(zonedateToChrono(calcTimes(jd, -LN_SOLAR_CIVIL_HORIZON                      ).set   )); }; break;
    case ESunTrackerEvent::steEveningGoldenHourEnd            :
    case ESunTrackerEvent::steEveningBlueHourStart            :
    case ESunTrackerEvent::steEveningSunsetStart              : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_STANDART                          ).set   )); }; break;
    case ESunTrackerEvent::steEveningCivilTwilightStart       :
    case ESunTrackerEvent::steEveningSunsetEnd                : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_STANDART + HA_SUN_ANGULAR_SIZE    ).set   )); }; break;
    case ESunTrackerEvent::steEveningBlueHourEnd              :
    case ESunTrackerEvent::steEveningCivilTwilightEnd         : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_CIVIL                             ).set   )); }; break;
    case ESunTrackerEvent::steEveningNauticalTwilightStart    :
    case ESunTrackerEvent::steEveningAstronomicalTwilightStart:
    case ESunTrackerEvent::steEveningNauticalTwilightEnd      : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_NAUTIC                            ).set   )); }; break;
    case ESunTrackerEvent::steEveningAstronomicalTwilightEnd  : { return CDateTime(zonedateToChrono(calcTimes(jd, HA_HORIZON_ASTRONOMICAL                      ).set  ));  }; break;
  }
  return CDateTime();
}

}
}
