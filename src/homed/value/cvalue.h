#pragma once
#include <jsoncpp/json/json.h>
#include "homed/value/ccolor.h"
#include "datetime/entities/cdatetime.h"
#include <variant>
#include <string>
#include <chrono>
#include <optional>
#include <mutex>
#include <memory>
#include "log.h"
namespace ha
{
namespace homed
{

#define HA_CVALUE_CTOR(_type) \
  explicit CValue(const _type &value, CStorage *ps) \
    : m_value    (value  ), \
      m_timestamp(       ), \
      m_parentStorage(ps ), \
      m_vString  (nullptr), \
      m_vInt     (nullptr), \
      m_vDouble  (nullptr), \
      m_vBool    (nullptr), \
      m_vColor   (nullptr)  \
   { HA_LOG_DBG_CVALUE("CValue " #_type " constructor " << this << " empty? " << (m_value.index() == 0)); }



using TValue = std::variant<std::monostate, std::string, int, double, bool, CColor>;

class CStorage;
class CValue
{
public:
  explicit CValue();
  explicit CValue(CStorage *ps);
  explicit CValue(const TValue &value, CStorage *ps);
           CValue(const CValue &other);
  HA_CVALUE_CTOR(int        );
  HA_CVALUE_CTOR(double     );
  HA_CVALUE_CTOR(bool       );
  HA_CVALUE_CTOR(std::string);
  HA_CVALUE_CTOR(CColor     );

  virtual ~CValue();

  const std::string &asString() const;
  const int         &asInt   () const;
  const double      &asDouble() const;
  const bool        &asBool  () const;
  const CColor      &asColor () const;

  bool isString() const { return std::holds_alternative<std::string>(m_value); }
  bool isInt   () const { return std::holds_alternative<int        >(m_value); }
  bool isDouble() const { return std::holds_alternative<double     >(m_value); }
  bool isBool  () const { return std::holds_alternative<bool       >(m_value); }
  bool isColor () const { return std::holds_alternative<CColor     >(m_value); }

  const TValue                  &value    () const { return m_value        ; }
  const ha::datetime::CDateTime &timestamp() const { return m_timestamp    ; }
  CStorage                      *storage  () const { return m_parentStorage; }

  bool    operator==(const CValue& other) const;
  bool    operator!=(const CValue& other) const;
  CValue& operator= (const CValue& other);

  // bool    operator==(const Json::Value& json) const;
  // bool    operator!=(const Json::Value& json) const;

  bool valueChanged(const CValue& other) const;

  Json::Value asJson(const std::string &key) const;

private:
  TValue m_value;
  ha::datetime::CDateTime m_timestamp;
  CStorage *m_parentStorage;

  mutable std::string *m_vString;
  mutable int         *m_vInt   ;
  mutable double      *m_vDouble;
  mutable bool        *m_vBool  ;
  mutable CColor      *m_vColor ;

  mutable std::mutex m_mutex;

};


}
}
