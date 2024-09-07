#include "includes/personal.as"
bool do_it = true;

void initialize()
{
  if(do_it)
  {
    logger.nfo("      OP: " + serial.gsm().operatorName());
    logger.nfo("  Signal: " + serial.gsm().signalLevel());
    logger.nfo(" Adapter: " + serial.gsm().adapterName());
    logger.nfo("Firmvare: " + serial.gsm().firmwareVersion());
    if(serial.gsm().sendSms(sheridan_phone_number, "⛩️ Тестовое сообщение из скрипта"))
    {
      logger.nfo("SMS sent to " + sheridan_phone_number);
    }
    if(serial.gsm().sendSms(wife_phone_number, "⛩️ Тестовое сообщение из скрипта"))
    {
      logger.nfo("SMS sent to " + wife_phone_number);
    }
  }
}

void deinitialize()
{}
