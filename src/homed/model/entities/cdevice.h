#pragma once
#include <vector>
#include <jsoncpp/json/json.h>
#include "mqtt/homed/ctopic.h"
#include "homed/base/cbase.h"
#include "homed/model/entities/cendpoint.h"
#include "homed/model/containers/cendpoints.h"

namespace ha
{
namespace homed
{

enum EDeviceType
{
  dtZigbee,
  dtCustom,
  dtUnknown
};

class CInstance;
class CDevice : virtual public CBase, public CEndpoint
{
  HA_OBJECT_PROPERTY(std::string, firmware         , "unknown");
  HA_OBJECT_PROPERTY(std::string, manufacturerName , "unknown");
  HA_OBJECT_PROPERTY(std::string, modelName        , "unknown");
  HA_OBJECT_PROPERTY(std::string, description      , "unknown");
  HA_OBJECT_PROPERTY(bool       , interviewFinished, false    );
  HA_OBJECT_PROPERTY(bool       , supported        , false    );
  HA_OBJECT_PROPERTY(bool       , active           , false    );
  HA_OBJECT_PROPERTY(bool       , cloud            , false    );
  HA_OBJECT_PROPERTY(bool       , discovery        , false    );

public:
  explicit CDevice(const std::string &name, const EDeviceType &dType, CInstance *pInstance);
  virtual ~CDevice();
  const std::string &name() const override { return CBase::name(); };
  CInstance *instance() { return m_parentInstance; };

  CEndpoints *endpoints() { return m_endpoints; }
  const EDeviceType &type() const { return m_type; }
  const std::string typeAsService();
  std::string topicPath(const ha::mqtt::ETopic &topic);
  void updateProperties();

private:
  CEndpoints *m_endpoints;
  EDeviceType m_type;
  CInstance *m_parentInstance;
};


}
}
