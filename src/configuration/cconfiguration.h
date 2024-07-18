#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace ha
{
namespace configuration
{

// #define HA_CONF_OPTION_DECLARE(_name,_type) _type _name()

#define HA_CONF_OPTION(_name,_type,_1key,_2key,_convert,_default) \
private: \
  SOption<_type> m_##_name; \
public: \
const _type & _name() \
{ \
  if(!m_##_name.loaded) \
  { \
    if(m_json.isMember(_1key) && m_json[_1key].isMember(_2key)) { m_##_name.set(m_json[_1key][_2key]._convert()); } \
    else                                                        { m_##_name.set(_default); } \
  } \
  return m_##_name.option; \
}

template<typename T>
struct SOption
{
  T option;
  bool loaded;
  SOption() : loaded(false) {}
  void set(const T &value) { loaded = true; option = value; }
};

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

  const std::string &scriptingLocation();

private:
  Json::Value m_json;
  std::string m_scriptingLocation;

  HA_CONF_OPTION(scriptingLocationOption , std::string, "scripting", "location", asString, "homed"    );
};

}
}
