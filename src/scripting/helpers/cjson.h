#pragma once
#include <jsoncpp/json/json.h>
#include "utils/string.h"

namespace ha
{
namespace scripting
{
namespace helpers
{

class CJson : public Json::Value
{
public:
  CJson();
  CJson(const std::string &value);
  CJson(const CJson       &value);
  CJson(const Json::Value &value);

  CJson& operator[](int index);
  CJson& operator[](const std::string& key);

  bool   operator< (const CJson& other) const;
  bool   operator<=(const CJson& other) const;
  bool   operator>=(const CJson& other) const;
  bool   operator> (const CJson& other) const;
  bool   operator==(const CJson& other) const;
  bool   operator!=(const CJson& other) const;

  CJson& operator= (const CJson      & other);
  CJson& operator= (const std::string& other);
  CJson& operator= (const int        & other);
  CJson& operator= (const double     & other);
  CJson& operator= (const bool       & other);

  ha::utils::CStrings getMemberNames() const;
  CJson get(const std::string& key, const CJson& defaultValue) const;
  unsigned int size() const;

};

}
}
}
