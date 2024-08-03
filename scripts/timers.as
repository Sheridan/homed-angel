bool do_it = false;

CTimerContinuous @timer_continuous;
CTimerOneshot    @timer_oneshoot  ;
CTimerCron       @timer_cron      ;
CTimerCron       @timer_wd_z      ;
CTimerCron       @timer_hd_z      ;

void onTimerContinuous()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("on");
  timer_oneshoot.start();
  printNext();
}

void onTimerOneshoot()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("off");
}

void onTimerCron()
{
  logger.nfo(script_name + " cron timer triggered");
}

void onTimerMorning()
{
  logger.nfo(script_name + " morning timer triggered");
}


void onButton(CProperty @property)
{
  logger.nfo(property.last().asString());
  if(property.last().asString() == "singleClick")
  {
    timer_continuous.reset();
  }
}

void printNext()
{
  logger.nfo("Oneshoot interval "   + timer_oneshoot  .timeInterval().asString() + ", next: " + timer_oneshoot  .nextTrigger().asString("%Y.%m.%d %H:%M:%S"));
  logger.nfo("Continuous interval " + timer_continuous.timeInterval().asString() + ", next: " + timer_continuous.nextTrigger().asString("%Y.%m.%d %H:%M:%S"));
  logger.nfo("Cron interval "       + timer_cron      .timeInterval().asString() + ", next: " + timer_cron      .nextTrigger().asString("%Y.%m.%d %H:%M:%S"));

  logger.nfo("Cron wd interval "    + timer_wd_z      .timeInterval().asString() + ", next: " + timer_wd_z      .nextTrigger().asString("%Y.%m.%d %H:%M:%S"));
  logger.nfo("Cron hd interval "    + timer_hd_z      .timeInterval().asString() + ", next: " + timer_hd_z      .nextTrigger().asString("%Y.%m.%d %H:%M:%S"));
}

void initialize()
{
  if(do_it)
  {
    @timer_continuous = CTimerContinuous(script_name, "onTimerContinuous", 8000);
    @timer_oneshoot   = CTimerOneshot   (script_name, "onTimerOneshoot"  , 4000);
    @timer_cron       = CTimerCron      (script_name, "onTimerCron"      , "*/3 */2 * * * *");
    @timer_wd_z       = CTimerCron      (script_name, "onTimerMorning"   , "0 0 7 * * 1-5");
    @timer_hd_z       = CTimerCron      (script_name, "onTimerMorning"   , "0 0 11 * * 6-7");


    logger.nfo(script_name + " init");
    homed.property(dtZigbee, "Button_20", "2", "action").subscribe(script_name, "onButton", false);
    timer_continuous.start();
    timer_cron.start();
    timer_wd_z.start();
    timer_hd_z.start();
    printNext();
  }
}

void deinitialize()
{
  if(do_it)
  {
    homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("off");
    logger.nfo(script_name + " deinit");
  }
}
