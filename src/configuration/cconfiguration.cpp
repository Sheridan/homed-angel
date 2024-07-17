#include "configuration/cconfiguration.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "st.h"

namespace ha
{
namespace configuration
{

CConfiguration:: CConfiguration() {}
CConfiguration::~CConfiguration() {}

void CConfiguration::load(const std::string &filename)
{
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
}

std::string CConfiguration::scriptingLocation()
{
  std::string location = HA_ST.cmdLine().option<std::string>("scripts");
  if(location.empty()) { location = scriptingLocationOption(); }
  if(location.empty()) { location = "/var/lib/homed-angel/scripts"; }
  return location;
}

}
}
