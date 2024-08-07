#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "log.h"

namespace ha
{
namespace configuration
{

// #define HA_CONF_OPTION_DECLARE(_name,_type) _type _name()

#define HA_CONF_OPTION(_name,_type,_keys,_convert,_default) \
private: \
  SOption<_type> m_##_name; \
public: \
const _type & _name() \
{ \
  if(!m_##_name.loaded) \
  { \
    try { Json::Value value = extract(_keys); m_##_name.set(value._convert()); } \
    catch(...) { m_##_name.set(_default); } \
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

  HA_CONF_OPTION(mqttServer         , std::string, std::initializer_list<std::string>({ "mqtt"     , "server"               }), asString, "localhost");
  HA_CONF_OPTION(mqttPort           , int        , std::initializer_list<std::string>({ "mqtt"     , "port"                 }), asInt   , 1833       );
  HA_CONF_OPTION(mqttUser           , std::string, std::initializer_list<std::string>({ "mqtt"     , "user"                 }), asString, "exporter" );
  HA_CONF_OPTION(mqttPassword       , std::string, std::initializer_list<std::string>({ "mqtt"     , "password"             }), asString, "exporter" );
  HA_CONF_OPTION(mqttHomedTopic     , std::string, std::initializer_list<std::string>({ "mqtt"     , "homed_root_topic"     }), asString, "homed"    );
  HA_CONF_OPTION(mqttPublishInterval, int        , std::initializer_list<std::string>({ "mqtt"     , "publish_interval_ms"  }), asInt   , 50         );
  HA_CONF_OPTION(mqttPublishQOS     , int        , std::initializer_list<std::string>({ "mqtt"     , "publish_qos"          }), asInt   , 1          );

  HA_CONF_OPTION(historyCount   , int        , std::initializer_list<std::string>({ "scripting", "values_history_count" }), asInt   , 2          );

  HA_CONF_OPTION(latitude , double, std::initializer_list<std::string>({ "astro", "coordinates", "latitude" }), asDouble, 0);
  HA_CONF_OPTION(longitude, double, std::initializer_list<std::string>({ "astro", "coordinates", "longitude"}), asDouble, 0);
  HA_CONF_OPTION(altitude , double, std::initializer_list<std::string>({ "astro", "coordinates", "altitude" }), asDouble, 0);

  const std::string &scriptingLocation();

private:
  Json::Value m_json;
  std::string m_scriptingLocation;

  Json::Value extract(const std::initializer_list<std::string> &keys);

  HA_CONF_OPTION(scriptingLocationOption , std::string, std::initializer_list<std::string>({"scripting", "location"}), asString, "/var/lib/homed-angel/scripts");
};

}
}
