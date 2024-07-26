#pragma once
#include <string>
#include <vector>
#include <map>
#include "datetime/astronomical/ctracker.h"

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
  SSunTrackerItem(const ESunTrackerEvent &e, const std::string &sn, const std::string &fn) : event(e), scriptName(sn), functionName(fn) {}
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

  void subscribe(const ESunTrackerEvent &event, const std::string &scriptName, const std::string &functionName);
  CDateTime *getEventTime(const ha::datetime::ESunTrackerEvent &event);
  bool circumpolar();

private:
  std::vector<SSunTrackerItem> m_events;
  std::map<ESunTrackerEvent, CDateTime *> m_eventsTime;

  SSunTrackerTime calcTimes(const double &jd, const double &horizon);
  void check(const ESunTrackerEvent &event);
  void update(const ESunTrackerEvent &event);
  ln_zonedate calcEventTime(ESunTrackerEvent event);

  #ifdef HA_DEBUG
  void printEvents();
  #endif

};


}
}
