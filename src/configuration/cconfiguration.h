#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "log.h"

#include "configuration/cconfigurationoption.h"

namespace ha
{
namespace configuration
{


#define HA_CONF_OPTION(_name,_type) \
private: \
  CConfigurationOption<_type> *m_##_name; \
public: \
  _type &_name()         { return m_##_name->value(); } \
  bool   _name##Exists() { return m_##_name->exists(); } \

class CConfiguration
{
public:
  CConfiguration();
  virtual ~CConfiguration();

  void load(const std::string &filename);

  HA_CONF_OPTION(mqttServer         , std::string);
  HA_CONF_OPTION(mqttPort           , int        );
  HA_CONF_OPTION(mqttUser           , std::string);
  HA_CONF_OPTION(mqttPassword       , std::string);
  HA_CONF_OPTION(mqttHomedTopic     , std::string);
  HA_CONF_OPTION(mqttPublishInterval, int        );
  HA_CONF_OPTION(mqttPublishQOS     , int        );
  HA_CONF_OPTION(historyCount       , int        );
  HA_CONF_OPTION(latitude           , double     );
  HA_CONF_OPTION(longitude          , double     );
  HA_CONF_OPTION(altitude           , double     );
  HA_CONF_OPTION(gsmDevice          , std::string);
  HA_CONF_OPTION(gsmBaudrate        , int        );
  HA_CONF_OPTION(gsmSMSSendInterval , int        );

  const std::string &scriptingLocation();

private:
  Json::Value m_json;
  std::string m_scriptingLocation;
  void loadOptions();

  HA_CONF_OPTION(scriptingLocationOption , std::string);
};

}
}
