#pragma once
#include <jsoncpp/json/json.h>
#include "homed/value/ccolor.h"
#include <variant>
#include <string>
#include <chrono>
#include <optional>

namespace ha
{
namespace homed
{

using TValue = std::variant<std::string, int, double, bool, CColor>;
using TTimestamp = std::chrono::system_clock::time_point;

class CValue
{
public:
  explicit CValue(const TValue &value);
  virtual ~CValue();

  const std::string asString() const;
  const int         asInt   () const;
  const double      asDouble() const;
  const bool        asBool  () const;
  const CColor      asColor () const;

  bool isString() const { return std::holds_alternative<std::string>(m_value); }
  bool isInt   () const { return std::holds_alternative<int        >(m_value); }
  bool isDouble() const { return std::holds_alternative<double     >(m_value); }
  bool isBool  () const { return std::holds_alternative<bool       >(m_value); }
  bool isColor () const { return std::holds_alternative<CColor     >(m_value); }

  const TValue     &value    () const { return m_value    ; }
  const TTimestamp &timestamp() const { return m_timestamp; }

  bool operator==(const CValue& other) const;
  bool operator!=(const CValue& other) const;

  bool valueChanged(const CValue& other) const;

  Json::Value asJson(const std::string &key) const;

private:
  TValue m_value;
  TTimestamp m_timestamp;


};


}
}
