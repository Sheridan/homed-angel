bool do_it = false;
CDateTime dt();
CTimeInterval ti(1234567890);

void initialize()
{
  if(do_it)
  {
    logger.nfo("           now: " + dt.asString("%Y.%m.%d %H:%M:%S"));
    logger.nfo("      now date: " + dt.asString("%Y.%m.%d"));
    logger.nfo("      now time: " + dt.asString("%H:%M:%S"));
    logger.nfo("      interval: " + ti.asString());
    logger.nfo("now + interval: " + (dt + ti).asString("%Y.%m.%d %H:%M:%S"));
    logger.nfo("     string dt: " + CDateTime("2020.05.05 12:32:25").asString());
    logger.nfo("   string date: " + CDateTime("2024.05.05").asString());
    logger.nfo("   string time: " + CDateTime("12:32:22").asString());
    logger.nfo("   string time: " + CDateTime("23:32:22").asString());
  }
}

void deinitialize()
{
  if(do_it)
  {
    logger.nfo("Start time: " + dt.asString());
  }
}
