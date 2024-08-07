bool do_it = true;

void onButton1(CProperty @property)
{
  if(property.last().asString() == "singleClick")
  {
    homed.property(dtZigbee, "LightRelay_Hall", "1", "status").set("toggle");
  }
  else if(property.last().asString() == "doubleClick")
  {
    homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("toggle");
    homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("toggle");
    homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("toggle");
  }
  else if(property.last().asString() == "hold")
  {
    homed.property(dtZigbee, "LightRelay_Hall", "1", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("off");
    homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("off");
  }
}

void onButton2(CProperty @property)
{
  homed.property(dtZigbee, "LightRelay_Hall", "1", "status").set("off");
  sleep(1000);
  homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("off");
  sleep(1000);
  homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("off");
  sleep(1000);
  homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("off");
}

void onButton4(CProperty @property)
{
  logger.nfo(homed.property(dtZigbee, "PowerSocket_CCTVMonitor", "status").last().asString());
  homed.property(dtZigbee, "PowerSocket_CCTVMonitor", "status").set("toggle");
}

void initialize()
{
  if(do_it)
  {
    logger.nfo(script_name + " init");
    homed.property(dtZigbee, "Button_20", "1", "action").subscribe(script_name, "onButton1", false);
    homed.property(dtZigbee, "Button_20", "2", "action").subscribe(script_name, "onButton2", false);
    homed.property(dtZigbee, "Button_20", "4", "action").subscribe(script_name, "onButton4", false);
  }
}
