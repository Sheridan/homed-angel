bool do_it = true;
bool rgb_running = false;

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

void onButton4(CProperty @property)
{
  logger.nfo(homed.property(dtZigbee, "PowerSocket_CCTVMonitor", "status").last().asString());
  homed.property(dtZigbee, "PowerSocket_CCTVMonitor", "status").set("toggle");
}

void rndShowcase()
{
  int level = random(10,90);
  CColor clr("random");
  logger.nfo("level: " + level + ", color: " + clr.asHexString() + " (" + clr.asInt() + ")");
  homed.property(dtZigbee, "LedRGB_Showcase", "status").set("on");
  homed.property(dtZigbee, "LedRGB_Showcase", "level").set(level);
  homed.property(dtZigbee, "LedRGB_Showcase", "color").set(clr);
}

void onButton5(CProperty @property)
{
  rndShowcase();
}

void onButton6(CProperty @property)
{
  CStrings endpoints;
  endpoints.push_back("1");
  endpoints.push_back("2");
  endpoints.push_back("3");
  endpoints.push_back("4");
  endpoints = shuffle_strings(endpoints);
  for(int i = 0; i < endpoints.size(); i++)
  {
    logger.nfo("endpoint " + endpoints[i]);
    homed.property(dtZigbee, "LightRelay_Hall", endpoints[i], "status").set("toggle");
    sleep(100);
  }
}

void rgbLoop()
{
  while(rgb_running)
  {
    rndShowcase();
    sleep(1000);
  }
}

void onButton7(CProperty @property)
{
  rgb_running = !rgb_running;
  logger.nfo("run? " + rgb_running);
  // if(rgb_running) { rgbLoop(); }
  // else
  // {
  //   homed.property(dtZigbee, "LedRGB_Showcase", "status").set("off");
  // }
}

bool joinStatus = false;
void onButton8(CProperty @property)
{
  homed.instances().setJoin(joinStatus);
  joinStatus = !joinStatus;
}

void initialize()
{
  if(do_it)
  {
    homed.property(dtZigbee, "Button_20", "1", "action").subscribe(script_name, "onButton1", false);
    homed.property(dtZigbee, "Button_20", "2", "action").subscribe(script_name, "onButton2", false);
    homed.property(dtZigbee, "Button_20", "4", "action").subscribe(script_name, "onButton4", false);
    homed.property(dtZigbee, "Button_20", "5", "action").subscribe(script_name, "onButton5", false);
    homed.property(dtZigbee, "Button_20", "6", "action").subscribe(script_name, "onButton6", false);
    homed.property(dtZigbee, "Button_20", "7", "action").subscribe(script_name, "onButton7", false);
    homed.property(dtZigbee, "Button_20", "8", "action").subscribe(script_name, "onButton8", false);
  }
}

void deinitialize()
{
  rgb_running = false;
  homed.property(dtZigbee, "LedRGB_Showcase", "status").set("off");
}
