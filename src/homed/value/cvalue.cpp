#include "homed/value/cvalue.h"

namespace ha
{
namespace homed
{

CValue::CValue(const TValue &value)
  : m_value(value),
    m_timestamp(std::chrono::system_clock::now())
{}

CValue::~CValue() {}

const std::string CValue::asString() const
{
  if(isString()) { return                std::get<std::string>(m_value);                    }
  if(isInt())    { return std::to_string(std::get<int        >(m_value));                   }
  if(isDouble()) { return std::to_string(std::get<double     >(m_value));                   }
  if(isBool())   { return                std::get<bool       >(m_value) ? "true" : "false"; }
  if(isColor())  { return                std::get<CColor     >(m_value).asHexString();      }
  return "undefined";
}

const int CValue::asInt() const
{
  if(isString()) { return std::stoi       (std::get<std::string>(m_value));                 }
  if(isInt())    { return                  std::get<int        >(m_value);                  }
  if(isDouble()) { return static_cast<int>(std::get<double     >(m_value));                 }
  if(isBool())   { return static_cast<int>(std::get<bool       >(m_value));                 }
  if(isColor())  { return                  std::get<CColor     >(m_value).asInt();          }
  return 0;
}

const double CValue::asDouble() const
{
  if(isString()) { return std::stod          (std::get<std::string>(m_value));              }
  if(isInt())    { return static_cast<double>(std::get<int        >(m_value));              }
  if(isDouble()) { return                     std::get<double     >(m_value);               }
  if(isBool())   { return static_cast<double>(std::get<bool       >(m_value));              }
  if(isColor())  { return static_cast<double>(std::get<CColor     >(m_value).asInt());      }
  return 0;
}

const bool CValue::asBool() const
{
  if(isString()) { return                   std::get<std::string>(m_value).empty();         }
  if(isInt())    { return static_cast<bool>(std::get<int        >(m_value));                }
  if(isDouble()) { return static_cast<bool>(std::get<double     >(m_value));                }
  if(isBool())   { return                   std::get<bool       >(m_value);                 }
  if(isColor())  { return static_cast<bool>(std::get<CColor     >(m_value).asInt());        }
  return false;
}

const CColor CValue::asColor() const
{
  if(isString()) { return CColor(                 std::get<std::string>(m_value));                                    }
  if(isInt())    { return CColor(static_cast<int>(std::get<int        >(m_value)));                                   }
  if(isDouble()) { return CColor(static_cast<int>(std::get<double     >(m_value)));                                   }
  if(isBool())   { return                         std::get<bool       >(m_value) ? CColor(0,255,0) : CColor(255,0,0); }
  if(isColor())  { return                         std::get<CColor     >(m_value);                                     }
  return CColor(0,0,0);
}

bool CValue::operator==(const CValue &other) const
{
  return m_value == other.m_value && m_timestamp == other.m_timestamp;
}

bool CValue::operator!=(const CValue &other) const
{
    return m_value != other.m_value || m_timestamp != other.m_timestamp;
}

bool CValue::valueChanged(const CValue &other) const
{
  return m_value != other.m_value;
}

Json::Value CValue::asJson(const std::string &key) const
{
    Json::Value jsonValue;
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
             if constexpr (std::is_same_v<T, int>        ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, double>     ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, bool>       ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, std::string>) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, CColor>     ) { jsonValue[key] = arg.toJson(); }
    }, m_value);
    return jsonValue;
}

}
}
