bool do_it = false;
void otaRefresh(string device)
{
  logger.nfo("Query refresh for " + device);
  CJson json();
  json["device"] = device;
  json["action"] = "otaRefresh";
  mqtt.publish("homed/command/zigbee", json.toStyledString());
}

void onDeviceActive(CProperty @property)
{
  otaRefresh(property.device().name());
  property.unsubscribe(script_name, "onDeviceActive");
}

void listDevices()
{
  CDevices @ds = homed.devices(dtZigbee);
  for(uint16 i = 0; i < ds.size(); i++)
  {
    CDevice @d = ds.get(i);
    // otaRefresh(d.name());
    homed.property(dtZigbee, d.name(), "lastSeen").subscribe(script_name, "onDeviceActive", false);
  }
}


void initialize()
{
  if(do_it)
  {
    listDevices();
  }
}

void deinitialize()
{
}
