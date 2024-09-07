#include "configuration/cconfiguration.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "st.h"
#include "cconfiguration.h"

namespace ha
{
namespace configuration
{


#define HA_CONF_OPTION_INIT(_name) m_##_name = nullptr;
CConfiguration:: CConfiguration()
  : m_scriptingLocation("")
{
  HA_CONF_OPTION_INIT(mqttServer             );
  HA_CONF_OPTION_INIT(mqttPort               );
  HA_CONF_OPTION_INIT(mqttUser               );
  HA_CONF_OPTION_INIT(mqttPassword           );
  HA_CONF_OPTION_INIT(mqttHomedTopic         );
  HA_CONF_OPTION_INIT(mqttPublishInterval    );
  HA_CONF_OPTION_INIT(mqttPublishQOS         );
  HA_CONF_OPTION_INIT(historyCount           );
  HA_CONF_OPTION_INIT(latitude               );
  HA_CONF_OPTION_INIT(longitude              );
  HA_CONF_OPTION_INIT(altitude               );
  HA_CONF_OPTION_INIT(gsmDevice              );
  HA_CONF_OPTION_INIT(gsmBaudrate            );
  HA_CONF_OPTION_INIT(gsmSMSSendInterval     );
  HA_CONF_OPTION_INIT(scriptingLocationOption);
}

#define HA_CONF_OPTION_DELETE(_name) delete m_##_name;
CConfiguration::~CConfiguration()
{
  HA_CONF_OPTION_DELETE(mqttServer             );
  HA_CONF_OPTION_DELETE(mqttPort               );
  HA_CONF_OPTION_DELETE(mqttUser               );
  HA_CONF_OPTION_DELETE(mqttPassword           );
  HA_CONF_OPTION_DELETE(mqttHomedTopic         );
  HA_CONF_OPTION_DELETE(mqttPublishInterval    );
  HA_CONF_OPTION_DELETE(mqttPublishQOS         );
  HA_CONF_OPTION_DELETE(historyCount           );
  HA_CONF_OPTION_DELETE(latitude               );
  HA_CONF_OPTION_DELETE(longitude              );
  HA_CONF_OPTION_DELETE(altitude               );
  HA_CONF_OPTION_DELETE(gsmDevice              );
  HA_CONF_OPTION_DELETE(gsmBaudrate            );
  HA_CONF_OPTION_DELETE(gsmSMSSendInterval     );
  HA_CONF_OPTION_DELETE(scriptingLocationOption);
}

void CConfiguration::load(const std::string &filename)
{
  HA_LOG_VERBOSE("Loading configuration from " << filename);
  std::ifstream ifs(filename, std::ifstream::binary);
  if (!ifs.is_open())
  {
    HA_ABORT("Can not open config file " + filename);
  }

  Json::CharReaderBuilder readerBuilder;
  std::string errs;
  if (!Json::parseFromStream(readerBuilder, ifs, &m_json, &errs))
  {
    HA_LOG_ERR("Error parsing config file: " << errs);
    HA_ABORT("Can not parse config file");
  }
  loadOptions();
}

#define HA_CONF_OPTION_LOAD(_name,_type,_path,_default) \
 m_##_name = new CConfigurationOption<_type>(&m_json, std::initializer_list<std::string> _path, _default);
void CConfiguration::loadOptions()
{
  HA_LOG_VERBOSE("Loading options...");
  HA_CONF_OPTION_LOAD(mqttServer              , std::string, ({ "mqtt"     , "server"              }), "localhost"                   );
  HA_CONF_OPTION_LOAD(mqttPort                , int        , ({ "mqtt"     , "port"                }), 1833                          );
  HA_CONF_OPTION_LOAD(mqttUser                , std::string, ({ "mqtt"     , "user"                }), "exporter"                    );
  HA_CONF_OPTION_LOAD(mqttPassword            , std::string, ({ "mqtt"     , "password"            }), "exporter"                    );
  HA_CONF_OPTION_LOAD(mqttHomedTopic          , std::string, ({ "mqtt"     , "homed_root_topic"    }), "homed"                       );
  HA_CONF_OPTION_LOAD(mqttPublishInterval     , int        , ({ "mqtt"     , "publish_interval_ms" }), 50                            );
  HA_CONF_OPTION_LOAD(mqttPublishQOS          , int        , ({ "mqtt"     , "publish_qos"         }), 1                             );
  HA_CONF_OPTION_LOAD(historyCount            , int        , ({ "scripting", "values_history_count"}), 2                             );
  HA_CONF_OPTION_LOAD(latitude                , double     , ({ "astro", "coordinates", "latitude" }), 0                             );
  HA_CONF_OPTION_LOAD(longitude               , double     , ({ "astro", "coordinates", "longitude"}), 0                             );
  HA_CONF_OPTION_LOAD(altitude                , double     , ({ "astro", "coordinates", "altitude" }), 0                             );
  HA_CONF_OPTION_LOAD(gsmDevice               , std::string, ({ "gsm", "device"                    }), "/dev/ttyUSB0"                );
  HA_CONF_OPTION_LOAD(gsmBaudrate             , int        , ({ "gsm", "baudrate"                  }), 115200                        );
  HA_CONF_OPTION_LOAD(gsmSMSSendInterval      , int        , ({ "gsm", "sms_min_send_interval_ms"  }), 5000                          );
  HA_CONF_OPTION_LOAD(scriptingLocationOption , std::string, ({"scripting", "location"             }), "/var/lib/homed-angel/scripts");
}

const std::string &CConfiguration::scriptingLocation()
{
  if(m_scriptingLocation.empty())
  {
    m_scriptingLocation = HA_ST->cmdLine()->option<std::string>("scripts");
    if(m_scriptingLocation.empty()) { m_scriptingLocation = scriptingLocationOption(); }
  }
  return m_scriptingLocation;
}


}
}
