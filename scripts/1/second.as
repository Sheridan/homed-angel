#include "../includes/include.has"
// int a = 0;

void doLog(CProperty @property, string caption)
{
  logger.nfo("{" + caption + "} " + property.device().name() +  " value: (" + property.type() + ") " + property.last().asString());
}

void allEvents(CProperty @property)
{
  doLog(property, "all");
}

void changedOnly(CProperty @property)
{
  doLog(property, "changed");
  homed.property(dtZigbee, "Informer_One", "1", "status").set("toggle");
}

void initialize()
{
  homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "allEvents"  , false);
  homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "allEvents"  , false);

  homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "changedOnly", true);
  homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "changedOnly", true);
  homed.property(dtZigbee, "Temperature_HallOutDoor"     , "temperature").subscribe(script_name, "changedOnly", true);
  homed.property(dtZigbee, "HumanDetector_BedStairs"     , "occupancy"  ).subscribe(script_name, "changedOnly", true);
  homed.property(dtZigbee, "DoorSensor_Hall"             , "contact"    ).subscribe(script_name, "changedOnly", true);
  homed.property(dtZigbee, "HumanDetector_Kitchen"       , "occupancy"  ).subscribe(script_name, "changedOnly", true);

  test_include();
  for( int n = 0; n <= 4; n++ )
  {
    logger.nfo('First logs ' + n);
  }
}
