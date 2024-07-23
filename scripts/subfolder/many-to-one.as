#include "../includes/include.as"
bool do_it = false;

void doLog(CProperty @property, string caption)
{
  logger.nfo("{" + caption + "} " + property.device().name() +  " value: (" + property.type() + ") " + property.last().asString());
}

void allEvents(CProperty @property)
{
  doLog(property, "all");
}

void toggle(CProperty @status, CProperty @color)
{
  if(status.last().asString() == "off")
  {
    color.set(CColor("random"));
    status.set("on");
  }
  else
  {
    status.set("off");
  }
}
void changedOnly(CProperty @property)
{
  doLog(property, "changed");
  toggle(homed.property(dtZigbee, "Informer_One", "1", "status"), homed.property(dtZigbee, "Informer_One", "1", "color"));
  toggle(homed.property(dtZigbee, "LedRGB_Showcase", "status"), homed.property(dtZigbee, "LedRGB_Showcase", "color"));
}

void initialize()
{
  if(do_it)
  {
    homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "allEvents"  , false);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "allEvents"  , false);

    homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "lastSeen"   ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "Temperature_HallOutDoor"     , "temperature").subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BedStairs"     , "occupancy"  ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BedStairs"     , "lastSeen"  ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BathRoom"     , "occupancy"  ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BathRoom"     , "lastSeen"  ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "DoorSensor_Hall"             , "contact"    ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_Kitchen"       , "occupancy"  ).subscribe(script_name, "changedOnly", true);
  }
  test_include();
  for( int n = 0; n <= 4; n++ )
  {
    logger.nfo('First logs ' + n);
  }
}
