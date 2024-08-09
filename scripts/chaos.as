bool do_it = false;
CTimerContinuous @timer;
void onButton2(CProperty @property)
{
  homed.property(dtZigbee, "LightRelay_Hall", "1", "status").set("off");
  homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("off");
  homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("off");
  homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("off");
}

void onTimer()
{
  string endpoint = to_string(random(1, 4));
  // logger.nfo(endpoint);
  CProperty @p = homed.property(dtZigbee, "LightRelay_Hall", endpoint, "status");
  p.set(p.last().asString() == "on" ? "off" : "on");
}

void initialize()
{
  if(do_it)
  {
    @timer = CTimerContinuous(script_name, "onTimer", 4000);
    timer.start();
  }
}

void deinitialize()
{
    homed.property(dtZigbee, "LightRelay_Hall", "1", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("off");
}
