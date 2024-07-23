#pragma once
#include <string>
#include <vector>
#include <array>
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
  void subscribe(const ESunTrackerEvent &event, const std::string &scriptName, const std::string &functionName);

private:
  std::vector<SSunTrackerItem> m_events;

  SSunTrackerTime calcTimes(const double &jd, const double &horizon);
  CDateTime calcTime(const ESunTrackerEvent &event);
  void checkEvent(const ESunTrackerEvent &event);

  #ifdef HA_DEBUG
  void printEvents();
  #endif

};


}
}
