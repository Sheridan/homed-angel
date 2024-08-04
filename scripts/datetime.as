
CDateTime dt();
CTimeInterval ti(3600);

void initialize()
{
  logger.nfo(script_name + " init");
  logger.nfo(dt.asString("%Y.%m.%d %H:%M:%S"));
  logger.nfo(ti.asString());
}

void deinitialize()
{
  logger.nfo(script_name + " deinit");
}
