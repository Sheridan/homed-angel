#pragma once
#include "tools/singletone/singletone.h"
#include "configuration/ccmdoptions.h"
#include "configuration/cconfiguration.h"
#include "mqtt/cmqttclient.h"
#include "scripting/cangel.h"
// #include "datetime/timers/ctimermanager.h"
#include "homed/chomed.h"

namespace ha
{

class CSingleTone
{
  HA_SIMPLE_SINGLTON_DECLARATION(ha::configuration::CCmdLine        , cmdLine)
  HA_SIMPLE_SINGLTON_DECLARATION(ha::configuration::CConfiguration  , config )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::scripting::CAngel              , angel  )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::homed::CHomed                  , homed  )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::mqtt::СMqttClient              , mqtt   )
  // HA_SIMPLE_SINGLTON_DECLARATION(ha::datetime::CTimerManager        , timers )
public:
  static CSingleTone &instance();
  void destruct();
  void sleep(const unsigned int &seconds = 100);

private:
  CSingleTone();
  ~CSingleTone();
};

}
