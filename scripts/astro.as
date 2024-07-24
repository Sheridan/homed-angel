
CTimerContinuous @timer_continuous = CTimerContinuous(script_name, "onTimer", 60 * 1000);

void printEventTime(ESunTrackerEvent event, string name)
{
  logger.nfo(astro.sun().getEventTime(event).asString("%Y.%m.%d %H:%M:%S") + " | " + name);
}

void onTimer()
{
  printEventTime(steNadir                           , "Nadir");
  printEventTime(steMorningBlueHourStart            , "MorningBlueHourStart");
  printEventTime(steMorningAstronomicalTwilightStart, "MorningAstronomicalTwilightStart");
  printEventTime(steMorningAstronomicalTwilightEnd  , "MorningAstronomicalTwilightEnd");
  printEventTime(steMorningNauticalTwilightStart    , "MorningNauticalTwilightStart");
  printEventTime(steMorningNauticalTwilightEnd      , "MorningNauticalTwilightEnd");
  printEventTime(steMorningCivilTwilightStart       , "MorningCivilTwilightStart");
  printEventTime(steMorningCivilTwilightEnd         , "MorningCivilTwilightEnd");
  printEventTime(steMorningBlueHourEnd              , "MorningBlueHourEnd");
  printEventTime(steMorningGoldenHourStart          , "MorningGoldenHourStart");
  printEventTime(steMorningSunriseStart             , "MorningSunriseStart");
  printEventTime(steMorningSunriseEnd               , "MorningSunriseEnd");
  printEventTime(steMorningGoldenHourEnd            , "MorningGoldenHourEnd");
  printEventTime(steZenith                          , "Zenith");
  printEventTime(steEveningGoldenHourStart          , "EveningGoldenHourStart");
  printEventTime(steEveningGoldenHourEnd            , "EveningGoldenHourEnd");
  printEventTime(steEveningBlueHourStart            , "EveningBlueHourStart");
  printEventTime(steEveningSunsetStart              , "EveningSunsetStart");
  printEventTime(steEveningSunsetEnd                , "EveningSunsetEnd");
  printEventTime(steEveningBlueHourEnd              , "EveningBlueHourEnd");
  printEventTime(steEveningCivilTwilightStart       , "EveningCivilTwilightStart");
  printEventTime(steEveningCivilTwilightEnd         , "EveningCivilTwilightEnd");
  printEventTime(steEveningNauticalTwilightStart    , "EveningNauticalTwilightStart");
  printEventTime(steEveningNauticalTwilightEnd      , "EveningNauticalTwilightEnd");
  printEventTime(steEveningAstronomicalTwilightStart, "EveningAstronomicalTwilightStart");
  printEventTime(steEveningAstronomicalTwilightEnd  , "EveningAstronomicalTwilightEnd");
}

void initialize()
{
  logger.nfo(script_name + " init");
  onTimer();
  timer_continuous.start();
}
