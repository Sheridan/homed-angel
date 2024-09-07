#include "configuration/cconfigurationoption.h"

namespace ha
{
namespace configuration
{

template <typename T>
CConfigurationOption<T>::CConfigurationOption(Json::Value *json, const std::initializer_list<std::string> &path, const T &defaultValue)
    : m_value(nullptr),
      m_default(defaultValue),
      m_exists(false),
      m_path(path),
      m_json(json)
{
  extract();
}

template <typename T>
CConfigurationOption<T>::~CConfigurationOption()
{
  delete m_value;
}

template <typename T>
void CConfigurationOption<T>::extract()
{
  Json::Value value = *m_json;
  for (const auto &key : m_path)
  {
    if (value.isMember(key)) { value = value[key]; }
    else
    {
      HA_LOG_WRN("Configuration key " << key << " not found");
      m_exists = false;
    }
  }
  m_value = new T(value.as<T>());
  m_exists = true;
}

template <typename T> T    &CConfigurationOption<T>::value       () { return m_value == nullptr ? m_default : *m_value; }
template <typename T> T    &CConfigurationOption<T>::defaultValue() { return m_default; }
template <typename T> bool  CConfigurationOption<T>::exists      () { return m_exists; }

}
}


namespace ha
{
namespace configuration
{
template class CConfigurationOption<std::string>;
template class CConfigurationOption<int>;
template class CConfigurationOption<double>;
}
}
