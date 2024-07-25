

CTimerContinuous @timer_continuous = CTimerContinuous(script_name, "onTimerContinuous", 8000);
CTimerOneshot    @timer_oneshoot   = CTimerOneshot   (script_name, "onTimerOneshoot"  , 4000);
void onTimerContinuous()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("on");
  timer_oneshoot.start();
}

void onTimerOneshoot()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("off");
}

void onButton(CProperty @property)
{
  logger.nfo(property.last().asString());
  if(property.last().asString() == "singleClick")
  {
    timer_continuous.reset();
  }
}

void initialize()
{
  logger.nfo(script_name + " init");
  homed.property(dtZigbee, "Button_One", "action").subscribe(script_name, "onButton", false);
  timer_continuous.start();
}

void deinitialize()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("off");
  logger.nfo(script_name + " deinit");
}
