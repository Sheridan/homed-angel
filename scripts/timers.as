

CTimerContinuous @timer = CTimerContinuous(script_name, "onTimer", 5000);

void onTimer()
{
  homed.property(dtZigbee, "LightRelay_Lobby", "1", "status").set("toggle");
}

void onButton(CProperty @property)
{
  logger.nfo(property.last().asString());
  if(property.last().asString() == "singleClick")
  {
    timer.reset();
  }
}

void initialize()
{
  homed.property(dtZigbee, "Button_One", "action").subscribe(script_name, "onButton", false);
  timer.start();
}
