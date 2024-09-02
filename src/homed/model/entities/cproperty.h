#pragma once
#include "homed/base/cbase.h"
#include "homed/value/cstorage.h"
#include "homed/value/ccolor.h"
#include "utils/string.h"

namespace ha
{
namespace homed
{

enum EPropertyValueType
{
  pvtEnum,
  pvtString,
  pvtBool,
  pvtDouble,
  pvtInt,
  pvtColor,
  pvtUnknown
};

class CDevice;
class CEndpoint;

class CProperty : public CBase
{
  friend class CEndpoint;

  HA_OBJECT_PROPERTY(bool               , readonly , true                          );
  HA_OBJECT_PROPERTY(EPropertyValueType , valueType, EPropertyValueType::pvtUnknown);
  HA_OBJECT_PROPERTY(ha::utils::CStrings, enumerate, {}                            );
  HA_OBJECT_PROPERTY(std::string        , unit     , ""                            );

  HA_OBJECT_PROPERTY(double, min , 0);
  HA_OBJECT_PROPERTY(double, max , 0);
  HA_OBJECT_PROPERTY(double, step, 0);

public:
  using CBase::name;
  explicit CProperty(const std::string &name, CDevice *pDevice, CEndpoint *pEndpoint);
  virtual ~CProperty();

  const CValue     &last() const;
  const std::string type() const;
  CValues history();
  CStorage *storage();

  void subscribe(const std::string &scriptName, const std::string &methodName, const bool &changedOnly);
  void unsubscribe(const std::string &scriptName, const std::string &methodName);

  CDevice   *device  () { return m_parentDevice  ; }
  CEndpoint *endpoint() { return m_parentEndpoint; }

  void set(const std::string &value);
  void set(const int         &value);
  void set(const double      &value);
  void set(const bool        &value);
  void set(const CColor      &value);
  void set(const CValue      &value);

  void setIfNotEqual(const std::string &value);
  void setIfNotEqual(const int         &value);
  void setIfNotEqual(const double      &value);
  void setIfNotEqual(const bool        &value);
  void setIfNotEqual(const CColor      &value);
  void setIfNotEqual(const CValue      &value);

private:
  CStorage *m_storage;
  CDevice *m_parentDevice;
  CEndpoint *m_parentEndpoint;


};



}
}
