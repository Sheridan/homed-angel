bool do_it = false;

void listProperties(CProperties @ps)
{
  logger.nfo("  Properties count: " + ps.size());
  for(uint16 i = 0; i < ps.size(); i++)
  {
    CProperty @p = ps.get(i);
    string text = p.name();
    if(!p.storage().empty())
    {
      text += " " + p.type() + ": " + p.last().asString();
    }
    logger.nfo("   " + text);
    if(p.valueType() == EPropertyValueType::pvtEnum)
    {
      CStrings enumerates = p.enumerate();
      for(int16 j = 0; j < enumerates.size(); j++)
      {
        logger.nfo("     " + enumerates[j]);
      }
    }
  }
}

void listEndpoints(CEndpoints @es)
{
  logger.nfo(" Endpoints count: " + es.size());
  for(uint16 i = 0; i < es.size(); i++)
  {
    CEndpoint @e = es.get(i);
    logger.nfo(" endpoint: " + e.name());
    listProperties(e.properties());
  }
}

void listDevices(CInstance @isc, EDeviceType dt)
{
  logger.nfo("Instance: " + (isc.name() == "" ? "N/A" : isc.name()));
  CDevices @ds = isc.devices(dt);
  logger.nfo("Devices count: " + ds.size());
  for(uint16 i = 0; i < ds.size(); i++)
  {
    CDevice @d = ds.get(i);
    logger.nfo("Name: " + d.name() + " | "+ d.description() + " | "+ d.modelName());
    listProperties(d.properties());
    listEndpoints(d.endpoints());
    logger.nfo("---");
  }
}

void listInstances()
{
  CInstances @iscs = homed.instances();
  logger.nfo("Instances count: " + iscs.size());
  for(uint16 i = 0; i < iscs.size(); i++)
  {
    logger.nfo("-------- Zigbee devices --------");
    listDevices(iscs.get(i), dtZigbee);
    logger.nfo("-------- Custom devices --------");
    listDevices(iscs.get(i), dtCustom);
  }
}

void initialize()
{
  if(do_it)
  {
    logger.nfo(script_name + " init");
    listInstances();
  }
}
