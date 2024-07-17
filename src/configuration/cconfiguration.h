#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace ha
{
namespace configuration
{

// #define HA_CONF_OPTION_DECLARE(_name,_type) _type _name()

#define HA_CONF_OPTION(_name,_type,_1key,_2key,_convert,_default) \
_type _name() \
{ \
  if(m_json.isMember(_1key) && m_json[_1key].isMember(_2key)) \
  { \
    return m_json[_1key][_2key]._convert(); \
  } \
  return _default; \
}

class CConfiguration
{
public:
  CConfiguration();
  virtual ~CConfiguration();

  void load(const std::string &filename);

  HA_CONF_OPTION(mqttServer     , std::string, "mqtt"     , "server"              , asString, "localhost");
  HA_CONF_OPTION(mqttPort       , int        , "mqtt"     , "port"                , asInt   , 1833       );
  HA_CONF_OPTION(mqttUser       , std::string, "mqtt"     , "user"                , asString, "exporter" );
  HA_CONF_OPTION(mqttPassword   , std::string, "mqtt"     , "password"            , asString, "exporter" );
  HA_CONF_OPTION(mqttHomedTopic , std::string, "mqtt"     , "homed_root_topic"    , asString, "homed"    );
  HA_CONF_OPTION(historyCount   , int        , "scripting", "values_history_count", asInt   , 2          );

  std::string scriptingLocation();

private:
  Json::Value m_json;

  HA_CONF_OPTION(scriptingLocationOption , std::string, "scripting", "location", asString, "homed"    );
};

}
}
