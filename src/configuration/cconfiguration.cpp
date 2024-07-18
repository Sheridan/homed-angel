#include "configuration/cconfiguration.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "st.h"

namespace ha
{
namespace configuration
{

CConfiguration:: CConfiguration() : m_scriptingLocation("") {}
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

const std::string &CConfiguration::scriptingLocation()
{
  if(m_scriptingLocation.empty())
  {
    m_scriptingLocation = HA_ST.cmdLine().option<std::string>("scripts");
    if(m_scriptingLocation.empty()) { m_scriptingLocation = scriptingLocationOption(); }
    if(m_scriptingLocation.empty()) { m_scriptingLocation = "/var/lib/homed-angel/scripts"; }
  }
  return m_scriptingLocation;
}

}
}
