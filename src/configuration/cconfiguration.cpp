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

CConfiguration:: CConfiguration() : m_scriptingLocation("") {}
CConfiguration::~CConfiguration() {}

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

Json::Value CConfiguration::extract(const std::initializer_list<std::string> &keys)
{
  Json::Value value = m_json;
  for (const auto &key : keys)
  {
    if (value.isMember(key)) { value = value[key]; }
    else { HA_LOG_WRN("Configuration key " << key << " not found"); throw 1; }
  }
  return value;
}

}
}
