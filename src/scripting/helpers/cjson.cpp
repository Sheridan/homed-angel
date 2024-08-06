#include "scripting/helpers/cjson.h"
#include "st.h"

namespace ha
{
namespace scripting
{
namespace helpers
{

CJson::CJson()                         : Json::Value(     ) {}
CJson::CJson(const CJson &value)       : Json::Value(value) {}
CJson::CJson(const Json::Value &value) : Json::Value(value) {}
CJson::CJson(const std::string &value) : Json::Value()
{
  Json::CharReaderBuilder *builder = new Json::CharReaderBuilder();
  Json::CharReader *reader = builder->newCharReader();
  JSONCPP_STRING err;
  try
  {
    if (!reader->parse(value.c_str(), value.c_str() + value.length(), this, &err))
    {
      HA_LOG_WRN("Scripting json parse error: " << err);
    }
  }
  catch(...) { HA_LOG_WRN("Scripting json parse error"); }
  delete reader;
  delete builder;

}

CJson& CJson::operator[](int index)              { return static_cast<CJson&>(Json::Value::operator[](index)); }
CJson& CJson::operator[](const std::string& key) { return static_cast<CJson&>(Json::Value::operator[](key  )); }

bool CJson::operator< (const CJson& other) const { return Json::Value::operator< (other); }
bool CJson::operator<=(const CJson& other) const { return Json::Value::operator<=(other); }
bool CJson::operator>=(const CJson& other) const { return Json::Value::operator>=(other); }
bool CJson::operator> (const CJson& other) const { return Json::Value::operator> (other); }
bool CJson::operator==(const CJson& other) const { return Json::Value::operator==(other); }
bool CJson::operator!=(const CJson& other) const { return Json::Value::operator!=(other); }

CJson& CJson::operator=(const CJson      & other) { Json::Value::operator=(other); return *this; }
CJson& CJson::operator=(const std::string& other) { Json::Value::operator=(other); return *this; }
CJson& CJson::operator=(const int        & other) { Json::Value::operator=(other); return *this; }
CJson& CJson::operator=(const double     & other) { Json::Value::operator=(other); return *this; }
CJson& CJson::operator=(const bool       & other) { Json::Value::operator=(other); return *this; }


ha::utils::CStrings CJson::getMemberNames() const
{
  ha::utils::CStrings result;
  for(const std::string &name : Json::Value::getMemberNames())
  {
    result.push_back(name);
  }
  return result;
}


CJson CJson::get(const std::string& key, const CJson& defaultValue) const
{
  return CJson(Json::Value::get(key, defaultValue));
}

unsigned int CJson::size() const { return Json::Value::size(); }

}
}
}
