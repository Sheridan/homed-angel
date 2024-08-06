#pragma once
#include <string>
#include <jsoncpp/json/json.h>

namespace ha
{
namespace mqtt
{

class CPayload
{
public:
  CPayload();
  ~CPayload();

  bool parse(const std::string& payload);
  const Json::Value &value() const { return m_root; }

  void replaceKey(const std::string &key, const std::string &replacement);

  #ifdef HA_DEBUG
  void dump();
  #endif

private:
  Json::CharReaderBuilder *m_builder;
  Json::CharReader *m_reader;
  Json::Value m_root;

  bool validate(const std::string& payload);
};

}
}
