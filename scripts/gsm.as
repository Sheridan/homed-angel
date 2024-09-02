bool do_it = true;
CGsm @gsm;

void initialize()
{
  if(do_it)
  {
    @gsm = CGsm("/dev/ttyUSB0", br115200);
    while(!gsm.ready())
    {
      logger.wrn("[/dev/ttyUSB0] GSM is not ready");
      sleep(1000);
    }
    logger.nfo("      OP: " + gsm.operatorName());
    logger.nfo("  Signal: " + gsm.signalLevel());
    logger.nfo(" Adapter: " + gsm.adapterName());
    logger.nfo("Firmvare: " + gsm.firmwareVersion());
    if(gsm.sendSms("+71111111111", "⛩️ Тестовое сообщение из скрипта"))
    {
      logger.nfo("SMS sent");
    }
  }
}

void deinitialize()
{}
