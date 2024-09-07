#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "log.h"

namespace ha
{
namespace configuration
{

template<typename T>
class CConfigurationOption
{
public:
  CConfigurationOption(Json::Value *json, const std::initializer_list<std::string> &path, const T &defaultValue);
  ~CConfigurationOption();

  T &value();
  T &defaultValue();
  bool exists();

private:
  T   *m_value;
  T    m_default;
  bool m_exists;
  std::initializer_list<std::string> m_path;
  Json::Value *m_json;
  void extract();

};

}
}
