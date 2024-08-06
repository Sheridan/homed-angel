#pragma once
#include "log.h"
#include "configuration/ccmdoptions.h"
#include "configuration/cconfiguration.h"
#include "mqtt/cmqtt.h"
#include "scripting/cangel.h"
#include "datetime/astronomical/castronomical.h"
#include "homed/chomed.h"



#define HA_DEFAULT_SLEEP_MS 100
#define HA_SIMPLE_SINGLTON_DECLARATION(_type,_name) static _type *_name() { return createInstance<_type>(); }
namespace ha
{

class CSingleTone
{
public:
  static CSingleTone *instance();
  void destruct();
  void sleep(const unsigned int &seconds = HA_DEFAULT_SLEEP_MS);

  template<typename T>
  static T* createInstance()
  {
    static T instance;
    return &instance;
  }

  HA_SIMPLE_SINGLTON_DECLARATION(ha::configuration::CCmdLine        , cmdLine)
  HA_SIMPLE_SINGLTON_DECLARATION(ha::configuration::CConfiguration  , config )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::scripting::CAngel              , angel  )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::homed::CHomed                  , homed  )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::mqtt::CMqtt                    , mqtt   )
  HA_SIMPLE_SINGLTON_DECLARATION(ha::datetime::CAstronomical        , astro  )

  CSingleTone(const CSingleTone&) = delete;
  CSingleTone& operator=(const CSingleTone&) = delete;
  CSingleTone(CSingleTone&&) = delete;
  CSingleTone& operator=(CSingleTone&&) = delete;
private:
  CSingleTone() = default;
};

}
