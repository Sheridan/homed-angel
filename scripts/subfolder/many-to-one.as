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
  doLog(status, "status");
  doLog(color, "color");
  if (status.storage().empty()) { logger.err("status is empty!"); }
  if (color.storage().empty()) { logger.err("color is empty!"); }
  if (status.last().asString() == "off")
  {
    CColor clr("random");
    logger.nfo("color: " + clr.asHexString());
    color.set(clr);
    status.set("on");
  }
  else
  {
    status.set("off");
  }
}
void changedOnly(CProperty @property)
{
  if (property.storage().empty()) { logger.err("Storage is empty!"); return; }
  // logger.wrn(property.last().timestamp().asString("%Y.%m.%d %H:%M:%S"));
  doLog(property, "changed");
  toggle(homed.property(dtZigbee, "Informer_One", "1", "status"), homed.property(dtZigbee, "Informer_One", "1", "color"));
  toggle(homed.property(dtZigbee, "LedRGB_Showcase"  , "status"), homed.property(dtZigbee, "LedRGB_Showcase"  , "color"));
}

void initialize()
{
  if(do_it)
  {
    for( int n = 0; n <= 4; n++ )
    {
      logger.nfo('First logs ' + n);
    }

    logger.nfo(script_name + " init");
    homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "allEvents"  , false);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "allEvents"  , false);

    homed.property(dtZigbee, "LightRelay_BathMirror"       , "status"     ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "changedOnly", true);
    // homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "lastSeen"   ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "Temperature_HallOutDoor"     , "temperature").subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BedStairs"     , "occupancy"  ).subscribe(script_name, "changedOnly", true);
    // homed.property(dtZigbee, "HumanDetector_BedStairs"     , "lastSeen"   ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_BathRoom"      , "occupancy"  ).subscribe(script_name, "changedOnly", true);
    // homed.property(dtZigbee, "HumanDetector_BathRoom"      , "lastSeen"   ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "DoorSensor_Hall"             , "contact"    ).subscribe(script_name, "changedOnly", true);
    homed.property(dtZigbee, "HumanDetector_Kitchen"       , "occupancy"  ).subscribe(script_name, "changedOnly", true);
  }
}
