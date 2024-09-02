bool do_it = false;

void setIfNot(CProperty @property, string value) { if(property.last().asString() != value) { logger.nfo("Setting default value for " + property.name() + ": " + value); property.set(value); sleep(500); } }
void setIfNot(CProperty @property, bool   value) { if(property.last().asBool  () != value) { logger.nfo("Setting default value for " + property.name() + ": " + value); property.set(value); sleep(500); } }
void setIfNot(CProperty @property, int    value) { if(property.last().asInt   () != value) { logger.nfo("Setting default value for " + property.name() + ": " + value); property.set(value); sleep(500); } }

void bindDevice(string srcDevice, int srcEndpointID, int srcClusterID, string dstDevice, int dstEndpointID)
{
  logger.nfo("Binding " + srcDevice + " (endpoint:" + srcEndpointID + ", cluster:" + srcClusterID + ") to " + dstDevice + " (endpoint:" + dstEndpointID + ")");
  CJson json();
  json["action"]        = "bindDevice" ;
  json["device"]        = srcDevice    ;
  json["endpointId"]    = srcEndpointID;
  json["clusterId"]     = srcClusterID ;
  json["dstDevice"]     = dstDevice    ;
  json["dstEndpointId"] = dstEndpointID;
  mqtt.publish("homed/command/zigbee", json.toStyledString());
}

void bindStreetTempToLobby(CProperty @property)
{
  bindDevice("Temperature_HallOutDoor", 1, hex_to_int("402"), "Air_Lobby", 1);
  bindDevice("Temperature_HallOutDoor", 1, hex_to_int("405"), "Air_Lobby", 1);
  homed.property(dtZigbee, "Temperature_HallOutDoor", "temperature").unsubscribe(script_name, "bindStreetTempToLobby");
}

void bindStreetTempToBed(CProperty @property)
{
  bindDevice("Temperature_WaterPumpOutDoor",1, hex_to_int("402"), "Air_BedRoom", 1);
  bindDevice("Temperature_WaterPumpOutDoor",1, hex_to_int("405"), "Air_BedRoom", 1);
  homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").unsubscribe(script_name, "bindStreetTempToBed");
}

void setDefaultSwitchBehaviour(string name, string insicatorMode, string powerOnStatus, bool childLock, string switchType)
{
  logger.nfo("Setting default behaviour for " + name);
  if(homed.device(dtZigbee, name).properties().exists("indicatorMode")) { setIfNot(homed.property(dtZigbee, name, "indicatorMode"), insicatorMode); }
  if(homed.device(dtZigbee, name).properties().exists("powerOnStatus")) { setIfNot(homed.property(dtZigbee, name, "powerOnStatus"), powerOnStatus); }
  if(homed.device(dtZigbee, name).properties().exists("switchType"   )) { setIfNot(homed.property(dtZigbee, name, "switchType"   ), switchType   ); }
  if(homed.device(dtZigbee, name).properties().exists("childLock"    )) { setIfNot(homed.property(dtZigbee, name, "childLock"    ), childLock    ); }
}

void setDefaultTHBehaviour(string name, string displayMode, int tempLow, int tempHigh, int humLow, int humHigh)
{
  logger.nfo("Setting default behaviour for " + name);
  if(homed.device(dtZigbee, name).properties().exists("displayMode"    )) { setIfNot(homed.property(dtZigbee, name, "displayMode"    ), displayMode); }
  if(homed.device(dtZigbee, name).properties().exists("temperatureLow" )) { setIfNot(homed.property(dtZigbee, name, "temperatureLow" ), tempLow    ); }
  if(homed.device(dtZigbee, name).properties().exists("temperatureHigh")) { setIfNot(homed.property(dtZigbee, name, "temperatureHigh"), tempHigh   ); }
  if(homed.device(dtZigbee, name).properties().exists("humidityLow"    )) { setIfNot(homed.property(dtZigbee, name, "humidityLow"    ), humLow     ); }
  if(homed.device(dtZigbee, name).properties().exists("humidityHigh"   )) { setIfNot(homed.property(dtZigbee, name, "humidityHigh"   ), humHigh    ); }
}

void resetDevicesStatus()
{
  setIfNot(homed.property(dtZigbee, "Relay_WaterPump", "1", "status"), "off");
  setIfNot(homed.property(dtZigbee, "Relay_WaterPump", "2", "status"), "off");
}

void initialize()
{
  if(do_it)
  {
    logger.nfo("Initialize start");

    setDefaultSwitchBehaviour("PowerSocket_CCTVMonitor"     , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_KitchenFitoLamp" , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_KitchenNightLamp", "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_PrinterHome"     , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_SPCBackLight"    , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_SPCMonitors"     , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_SPComputer"      , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_SPCSound"        , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_SPCSubWoofer"    , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_Street"          , "default", "off"     , false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_WarmFloor"       , "default", "previous", false, ""      );
    setDefaultSwitchBehaviour("PowerSocket_WaterPump"       , "default", "previous", false, ""      );
    setDefaultSwitchBehaviour("LedRGB_BedStairs"            , ""       , "off"     , false, ""      );
    setDefaultSwitchBehaviour("LedRGB_Showcase"             , ""       , "off"     , false, ""      );
    setDefaultSwitchBehaviour("LightRelay_Bar"              , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_BathLamp"         , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_BathMirror"       , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_Hall"             , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_KitchenTable"     , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_KitchenWorktop"   , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("LightRelay_Lobby"            , "default", "off"     , false, "toggle");
    setDefaultSwitchBehaviour("Relay_WaterPump"             , "default", "off"     , false, "toggle");

    setDefaultTHBehaviour("Temperature_Hall"   , "celsius", 19, 26, 30, 50);
    setDefaultTHBehaviour("Temperature_BedRoom", "celsius", 19, 26, 30, 50);
    setDefaultTHBehaviour("Temperature_Kitchen", "celsius", 19, 26, 30, 50);

    homed.property(dtZigbee, "Temperature_HallOutDoor"     , "temperature").subscribe(script_name, "bindStreetTempToLobby", false);
    homed.property(dtZigbee, "Temperature_WaterPumpOutDoor", "temperature").subscribe(script_name, "bindStreetTempToBed"  , false);

    resetDevicesStatus();

    logger.nfo("Initialize done");
  }
}

void deinitialize()
{
  if(do_it)
  {
    for(int i = 1; i <= 8; i++)
    {
      setIfNot(homed.property(dtZigbee, "Informer_One", to_string(i), "status"), "off");
    }
    setIfNot(homed.property(dtZigbee, "Siren_Lobby", "alarm"), false);

    resetDevicesStatus();
  }
}
