class CDayTime
{
  const CDateTime checkToday(const CDateTime &in dt)
  {
    CDateTime result(dt);
    if(result.asString("%d") != CDateTime().asString("%d"))
    {
      result = result - CTimeInterval(24*60*60);
    }
    logger.nfo(result.asString());
    return result;
  }
  const CDateTime morningStart() { return checkToday(astro.sun().getEventTime(steMorningNauticalTwilightStart)); }
  const CDateTime morningEnd  () { return checkToday(astro.sun().getEventTime(steMorningSunriseEnd           )); }
  const CDateTime eveningStart() { return checkToday(astro.sun().getEventTime(steEveningSunsetStart          )); }
  const CDateTime eveningEnd  () { return checkToday(astro.sun().getEventTime(steEveningNauticalTwilightEnd  )); }

  bool isDay() { return isDay(CDateTime()); }
  bool isDay(const CDateTime &in dt) { return morningEnd().asUnixTimestamp() < dt.asUnixTimestamp() && dt.asUnixTimestamp() <= eveningStart().asUnixTimestamp(); };

  bool isNight() { return isNight(CDateTime()); }
  bool isNight(const CDateTime &in dt) { return eveningEnd().asUnixTimestamp() < dt.asUnixTimestamp() && dt.asUnixTimestamp() <= morningStart().asUnixTimestamp(); };

  bool isMorning() { return isMorning(CDateTime()); }
  bool isMorning(const CDateTime &in dt) { return morningStart().asUnixTimestamp() < dt.asUnixTimestamp() && dt.asUnixTimestamp() <= morningEnd().asUnixTimestamp(); };

  bool isEvening() { return isEvening(CDateTime()); }
  bool isEvening(const CDateTime &in dt) { return eveningStart().asUnixTimestamp() < dt.asUnixTimestamp() && dt.asUnixTimestamp() <= eveningEnd().asUnixTimestamp(); };
};
