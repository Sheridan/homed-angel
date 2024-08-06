#pragma once
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "datetime/astronomical/ctracker.h"
#include "log.h"

namespace ha
{
namespace datetime
{

enum ESunTrackerEvent
{
  steNadir,
  steMorningAstronomicalTwilightStart,
  steMorningAstronomicalTwilightEnd,
  steMorningNauticalTwilightStart,
  steMorningNauticalTwilightEnd,
  steMorningCivilTwilightStart,
  steMorningCivilTwilightEnd,
  steMorningBlueHourStart,
  steMorningBlueHourEnd,
  steMorningSunriseStart,
  steMorningSunriseEnd,
  steMorningGoldenHourStart,
  steMorningGoldenHourEnd,
  steZenith,
  steEveningGoldenHourStart,
  steEveningGoldenHourEnd,
  steEveningSunsetStart,
  steEveningSunsetEnd,
  steEveningBlueHourStart,
  steEveningBlueHourEnd,
  steEveningCivilTwilightStart,
  steEveningCivilTwilightEnd,
  steEveningNauticalTwilightStart,
  steEveningNauticalTwilightEnd,
  steEveningAstronomicalTwilightStart,
  steEveningAstronomicalTwilightEnd
  // steUnknown
};

struct SSunTrackerItem
{
  std::string scriptName;
  std::string functionName;
  ESunTrackerEvent event;
};

struct SSunTrackerTime
{
  ln_zonedate rise;
  ln_zonedate zenith;
  ln_zonedate set;
  ln_zonedate nadir;
  bool        circumpolar;
};

class CSunTracker : public CTracker
{
public:
  explicit CSunTracker();
  virtual ~CSunTracker();

  void update();
  void check();

  void subscribe(const std::string &scriptName, const std::string &functionName, const ESunTrackerEvent &event);
  void unsubscribe(const std::string &scriptName);

  const CDateTime &getEventTime(const ha::datetime::ESunTrackerEvent &event);
  bool circumpolar();

private:
  std::vector<SSunTrackerItem> m_events;
  std::map<ESunTrackerEvent, CDateTime> m_eventsTime;
  bool m_circumpolar;
  std::mutex m_mutex;

  SSunTrackerTime calcTimes(const double &jd, const double &horizon);
  void check(const ESunTrackerEvent &event);
  void update(const ESunTrackerEvent &event);
  ln_zonedate calcEventTime(ESunTrackerEvent event);

  #ifdef HA_ASTRO_DEBUG
  void printEvents();
  #endif

};


}
}
