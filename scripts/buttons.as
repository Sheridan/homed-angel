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
  homed.property(dtZigbee, "LightRelay_Hall", "2", "status").set("off");
  homed.property(dtZigbee, "LightRelay_Hall", "3", "status").set("off");
  homed.property(dtZigbee, "LightRelay_Hall", "4", "status").set("off");
}

void initialize()
{
  if(do_it)
  {
    logger.nfo(script_name + " init");
    homed.property(dtZigbee, "Button_20", "1", "action").subscribe(script_name, "onButton1", false);
    homed.property(dtZigbee, "Button_20", "2", "action").subscribe(script_name, "onButton2", false);
  }
}
