#include "classes/daytime.as"
bool do_it = true;

// CTimerContinuous @timer_continuous;

void printEventTime(ESunTrackerEvent event, string name)
{
  logger.nfo(astro.sun().getEventTime(event).asString("%Y.%m.%d %H:%M:%S") + " | " + name);
}

void onTimer()
{
  printEventTime(steNadir                           , "Nadir"                           );
  printEventTime(steMorningBlueHourStart            , "MorningBlueHourStart"            );
  printEventTime(steMorningAstronomicalTwilightStart, "MorningAstronomicalTwilightStart");
  printEventTime(steMorningAstronomicalTwilightEnd  , "MorningAstronomicalTwilightEnd"  );
  printEventTime(steMorningNauticalTwilightStart    , "MorningNauticalTwilightStart"    );
  printEventTime(steMorningNauticalTwilightEnd      , "MorningNauticalTwilightEnd"      );
  printEventTime(steMorningCivilTwilightStart       , "MorningCivilTwilightStart"       );
  printEventTime(steMorningCivilTwilightEnd         , "MorningCivilTwilightEnd"         );
  printEventTime(steMorningBlueHourEnd              , "MorningBlueHourEnd"              );
  printEventTime(steMorningGoldenHourStart          , "MorningGoldenHourStart"          );
  printEventTime(steMorningSunriseStart             , "MorningSunriseStart"             );
  printEventTime(steMorningSunriseEnd               , "MorningSunriseEnd"               );
  printEventTime(steMorningGoldenHourEnd            , "MorningGoldenHourEnd"            );
  printEventTime(steZenith                          , "Zenith"                          );
  printEventTime(steEveningGoldenHourStart          , "EveningGoldenHourStart"          );
  printEventTime(steEveningGoldenHourEnd            , "EveningGoldenHourEnd"            );
  printEventTime(steEveningBlueHourStart            , "EveningBlueHourStart"            );
  printEventTime(steEveningSunsetStart              , "EveningSunsetStart"              );
  printEventTime(steEveningSunsetEnd                , "EveningSunsetEnd"                );
  printEventTime(steEveningBlueHourEnd              , "EveningBlueHourEnd"              );
  printEventTime(steEveningCivilTwilightStart       , "EveningCivilTwilightStart"       );
  printEventTime(steEveningCivilTwilightEnd         , "EveningCivilTwilightEnd"         );
  printEventTime(steEveningNauticalTwilightStart    , "EveningNauticalTwilightStart"    );
  printEventTime(steEveningNauticalTwilightEnd      , "EveningNauticalTwilightEnd"      );
  printEventTime(steEveningAstronomicalTwilightStart, "EveningAstronomicalTwilightStart");
  printEventTime(steEveningAstronomicalTwilightEnd  , "EveningAstronomicalTwilightEnd"  );
}

void onEvent(ESunTrackerEvent event)
{
  logger.nfo("! Some astro event !" + event);
}

void subscribeEvent(ESunTrackerEvent event)
{
  astro.sun().subscribe(script_name, "onEvent", event);
}

void initialize()
{
  CDayTime d_time;
  if(d_time.isDay    ()) { logger.nfo("Day"    ); }
  if(d_time.isNight  ()) { logger.nfo("Night"  ); }
  if(d_time.isMorning()) { logger.nfo("Morning"); }
  if(d_time.isEvening()) { logger.nfo("Evening"); }
  if(do_it)
  {
    // @timer_continuous = CTimerContinuous(script_name, "onTimer", 60 * 1000);
    logger.nfo(script_name + " init");
    onTimer();
    // timer_continuous.start();


    subscribeEvent(steNadir                           );
    subscribeEvent(steMorningBlueHourStart            );
    subscribeEvent(steMorningAstronomicalTwilightStart);
    subscribeEvent(steMorningAstronomicalTwilightEnd  );
    subscribeEvent(steMorningNauticalTwilightStart    );
    subscribeEvent(steMorningNauticalTwilightEnd      );
    subscribeEvent(steMorningCivilTwilightStart       );
    subscribeEvent(steMorningCivilTwilightEnd         );
    subscribeEvent(steMorningBlueHourEnd              );
    subscribeEvent(steMorningGoldenHourStart          );
    subscribeEvent(steMorningSunriseStart             );
    subscribeEvent(steMorningSunriseEnd               );
    subscribeEvent(steMorningGoldenHourEnd            );
    subscribeEvent(steZenith                          );
    subscribeEvent(steEveningGoldenHourStart          );
    subscribeEvent(steEveningGoldenHourEnd            );
    subscribeEvent(steEveningBlueHourStart            );
    subscribeEvent(steEveningSunsetStart              );
    subscribeEvent(steEveningSunsetEnd                );
    subscribeEvent(steEveningBlueHourEnd              );
    subscribeEvent(steEveningCivilTwilightStart       );
    subscribeEvent(steEveningCivilTwilightEnd         );
    subscribeEvent(steEveningNauticalTwilightStart    );
    subscribeEvent(steEveningNauticalTwilightEnd      );
    subscribeEvent(steEveningAstronomicalTwilightStart);
    subscribeEvent(steEveningAstronomicalTwilightEnd  );
  }
}
