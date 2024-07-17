#include "../includes/include.has"
// int a = 0;

void doLog(CProperty @property, string caption)
{
  logger.nfo("{" + caption + "} " + property.device().name() +  " value: (" + property.storage().type() + ") " + property.last().asString());
}

void allEvents(CProperty @property)
{
  doLog(property, "all");
}

void changedOnly(CProperty @property)
{
  doLog(property, "changed");
  homed.devices(dtZigbee).get("Informer_One").endpoints().get("1").properties().get("status").set("toggle");
}

void initialize()
{
  // homed.devices(dtZigbee).get("LightRelay_BathMirror").properties().get("status").subscribe(script_name, "allEvents"  , false);
  // homed.devices(dtZigbee).get("LightRelay_BathMirror").properties().get("status").subscribe(script_name, "changedOnly", true);

  // homed.devices(dtZigbee).get("Temperature_WaterPumpOutDoor").properties().get("temperature").subscribe(script_name, "allEvents"  , false);
  // homed.devices(dtZigbee).get("Temperature_WaterPumpOutDoor").properties().get("temperature").subscribe(script_name, "changedOnly", true);

  // homed.devices(dtZigbee).get("Temperature_HallOutDoor").properties().get("temperature").subscribe(script_name, "allEvents"  , false);
  // homed.devices(dtZigbee).get("Temperature_HallOutDoor").properties().get("temperature").subscribe(script_name, "changedOnly", true);

  // homed.devices(dtZigbee).get("HumanDetector_BedStairs").properties().get("occupancy").subscribe(script_name, "allEvents"  , false);
  // homed.devices(dtZigbee).get("HumanDetector_BedStairs").properties().get("occupancy").subscribe(script_name, "changedOnly", true);

  homed.devices(dtZigbee).get("DoorSensor_Hall").properties().get("contact").subscribe(script_name, "allEvents"  , false);
  homed.devices(dtZigbee).get("DoorSensor_Hall").properties().get("contact").subscribe(script_name, "changedOnly", true);

  test_include();
  for( int n = 0; n <= 4; n++ )
  {
    logger.nfo('First logs ' + n);
  }
}
