#include "homed/value/cvalue.h"
#include "st.h"
#include "cvalue.h"
namespace ha
{
namespace homed
{
CValue::CValue(CStorage *ps)
  : m_value    (std::monostate{}),
    m_timestamp(),
    m_parentStorage(ps),
    m_vString  (nullptr),
    m_vInt     (nullptr),
    m_vDouble  (nullptr),
    m_vBool    (nullptr),
    m_vColor   (nullptr)
{}

CValue::CValue(const TValue &value, CStorage *ps)
  : m_value    (value  ),
    m_timestamp(       ),
    m_parentStorage(ps),
    m_vString  (nullptr),
    m_vInt     (nullptr),
    m_vDouble  (nullptr),
    m_vBool    (nullptr),
    m_vColor   (nullptr)
    // m_mutex    (std::make_unique<std::mutex>())
{
  HA_LOG_DBG_CVALUE("CValue TValue constructor " << this << " empty? " << (m_value.index() == 0));
}

CValue::CValue(const CValue &other)
  : m_value    (other.m_value),
    m_timestamp(other.m_timestamp),
    m_parentStorage(other.m_parentStorage),
    m_vString  (nullptr),
    m_vInt     (nullptr),
    m_vDouble  (nullptr),
    m_vBool    (nullptr),
    m_vColor   (nullptr)
{
  HA_LOG_DBG_CVALUE("CValue copy constructor " << this << " empty? " << (m_value.index() == 0));
}

CValue::~CValue()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("CValue delete " << this);
  delete m_vString;
  delete m_vInt;
  delete m_vDouble;
  delete m_vBool;
  delete m_vColor;
}


const std::string &CValue::asString() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("asString " << this);
  if(!m_vString)
  {
    std::string result = "undefined";
         if(isString()) { result =                std::get<std::string>(m_value)                   ; }
    else if(isInt   ()) { result = std::to_string(std::get<int        >(m_value))                  ; }
    else if(isDouble()) { result = std::to_string(std::get<double     >(m_value))                  ; }
    else if(isBool  ()) { result =                std::get<bool       >(m_value) ? "true" : "false"; }
    else if(isColor ()) { result =                std::get<CColor     >(m_value).asHexString()     ; }
    m_vString = new std::string(result);
  }
  HA_LOG_DBG_CVALUE("asString after " << *m_vString);
  return *m_vString;
}

const int &CValue::asInt() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("asInt " << this);
  if(!m_vInt)
  {
    int result = 0;
         if(isString()) { result = std::stoi       (std::get<std::string>(m_value))       ; }
    else if(isInt   ()) { result =                  std::get<int        >(m_value)        ; }
    else if(isDouble()) { result = static_cast<int>(std::get<double     >(m_value))       ; }
    else if(isBool  ()) { result = static_cast<int>(std::get<bool       >(m_value))       ; }
    else if(isColor ()) { result =                  std::get<CColor     >(m_value).asInt(); }
    m_vInt = new int(result);
  }
  HA_LOG_DBG_CVALUE("asInt after " << *m_vInt);
  return *m_vInt;
}

const double &CValue::asDouble() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("asDouble " << this);
  if(!m_vDouble)
  {
    double result = 0;
         if(isString()) { result = std::stod          (std::get<std::string>(m_value))        ; }
    else if(isInt   ()) { result = static_cast<double>(std::get<int        >(m_value))        ; }
    else if(isDouble()) { result =                     std::get<double     >(m_value)         ; }
    else if(isBool  ()) { result = static_cast<double>(std::get<bool       >(m_value))        ; }
    else if(isColor ()) { result = static_cast<double>(std::get<CColor     >(m_value).asInt()); }
    m_vDouble = new double(result);
  }
  HA_LOG_DBG_CVALUE("asDouble after " << *m_vDouble);
  return *m_vDouble;
}

const bool &CValue::asBool() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("asBool " << this);
  if(!m_vBool)
  {
    bool result = false;
         if(isString()) { result =                   std::get<std::string>(m_value).empty() ; }
    else if(isInt   ()) { result = static_cast<bool>(std::get<int        >(m_value))        ; }
    else if(isDouble()) { result = static_cast<bool>(std::get<double     >(m_value))        ; }
    else if(isBool  ()) { result =                   std::get<bool       >(m_value)         ; }
    else if(isColor ()) { result = static_cast<bool>(std::get<CColor     >(m_value).asInt()); }
    m_vBool = new bool(result);
  }
  HA_LOG_DBG_CVALUE("asBool after " << *m_vBool);
  return *m_vBool;
}

const CColor &CValue::asColor() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  HA_LOG_DBG_CVALUE("asColor "  << this);
  if(!m_vColor)
  {
    CColor result(0,0,0);
         if(isString()) { result = CColor(                 std::get<std::string>(m_value)                                   ); }
    else if(isInt   ()) { result = CColor(static_cast<int>(std::get<int        >(m_value))                                  ); }
    else if(isDouble()) { result = CColor(static_cast<int>(std::get<double     >(m_value))                                  ); }
    else if(isBool  ()) { result =                         std::get<bool       >(m_value) ? CColor(0,255,0) : CColor(255,0,0); }
    else if(isColor ()) { result = CColor(                 std::get<CColor     >(m_value)                                   ); }
    m_vColor = new CColor(result);
  }
  HA_LOG_DBG_CVALUE("asBool after " << m_vColor->asHexString());
  return *m_vColor;
}

bool CValue::operator==(const CValue &other) const
{
  return m_value == other.m_value && m_timestamp == other.m_timestamp;
}

bool CValue::operator!=(const CValue &other) const
{
  return m_value != other.m_value || m_timestamp != other.m_timestamp;
}

CValue &CValue::operator=(const CValue &other)
{
  if (this != &other)
  {
    // m_mutex = std::make_unique<std::mutex>();
    // std::lock_guard<std::mutex> lock(*m_mutex);
    m_value = other.m_value;
    m_timestamp = other.m_timestamp;
  }
  return *this;
}

// bool CValue::operator==(const Json::Value &json) const
// {
//   return std::visit([&json](auto&& arg) -> bool
//   {
//     using T = std::decay_t<decltype(arg)>;
//          if constexpr (std::is_same_v<T, std::string>) { return json.isString() && json.asString() == arg; }
//     else if constexpr (std::is_same_v<T, int>)         { return json.isInt()    && json.asInt()    == arg; }
//     else if constexpr (std::is_same_v<T, double>)      { return json.isDouble() && json.asDouble() == arg;}
//     else if constexpr (std::is_same_v<T, bool>)        { return json.isBool()   && json.asBool()   == arg; }
//     else if constexpr (std::is_same_v<T, CColor>)      { return json.isObject() && arg.toJson() == json; }
//     return false;
//   }, m_value);
// }

// bool CValue::operator!=(const Json::Value &json) const
// {
//   return !(*this == json);
// }

bool CValue::valueChanged(const CValue &other) const
{
  return m_value != other.m_value;
}

Json::Value CValue::asJson(const std::string &key) const
{
  // std::lock_guard<std::mutex> lock(*m_mutex);
  Json::Value jsonValue;
  std::visit(
    [&](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;
             if constexpr (std::is_same_v<T, int>        ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, double>     ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, bool>       ) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, std::string>) { jsonValue[key] = arg         ; }
        else if constexpr (std::is_same_v<T, CColor>     ) { jsonValue[key] = arg.toJson(); }
    },
    m_value);
  return jsonValue;
}

}
}
