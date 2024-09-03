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

void listInstances()
{
  CInstances @iscs = homed.instances();
  for(uint16 i = 0; i < iscs.size(); i++)
  {
    listDevices(iscs.get(i));
  }
}

void listDevices(CInstance @isc)
{
  CDevices @ds = isc.devices(dtZigbee);
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
    listInstances();
  }
}

void deinitialize()
{
}
