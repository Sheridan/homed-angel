#include "homed/model/entities/cendpoint.h"
#include "homed/model/entities/cdevice.h"
#include <ranges>
#include "st.h"

namespace ha
{
namespace homed
{

CEndpoint::CEndpoint(const std::string &name, CDevice *pDevice)
  : CBase(name),
    m_parentDevice(pDevice)
{
  m_properties = new CProperties(m_parentDevice, this);
}

CEndpoint::~CEndpoint()
{
  delete m_properties;
}

void CEndpoint::update(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  HA_LOG_DBG_INCOMING("Updating " << (topic->deviceEndpoint().empty() ? "device" : "endpoint") << " " << topic->device() << (topic->deviceEndpoint().empty() ? "" : ":" + topic->deviceEndpoint()) << " (" << topic->service() <<  ":" << topic->topic() << ")");
  switch(topic->topicType())
  {
    case ha::mqtt::ETopic::tStatus: updateStatus(topic, payload); break;
    case ha::mqtt::ETopic::tExpose: updateExpose(topic, payload); break;
    case ha::mqtt::ETopic::tFd:     updateFd    (topic, payload); break;
    case ha::mqtt::ETopic::tDevice: updateDevice(topic, payload); break;
  }
}

#define HA_SET_OBJECT_PROPERTY(_name, _as) if (payload.isMember(#_name)) { device->_name(payload[#_name]._as()); }
void CEndpoint::updateStatus(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  HA_LOG_DBG_INCOMING("Updating status: " << topic->device());
  CProperty *property = properties()->ensure("linkQuality");
  property->readonly(true);
  property->valueType(EPropertyValueType::pvtInt);
  property = properties()->ensure("lastSeen");
  property->readonly(true);
  property->valueType(EPropertyValueType::pvtInt);
  property = properties()->ensure("online");
  property->readonly(true);
  property->valueType(EPropertyValueType::pvtEnum);
  property->enumerate({"online", "offline"});

  CDevice *device = HA_ST->homed()->devices(topic->serviceType())->ensure(topic->device());
  HA_SET_OBJECT_PROPERTY(firmware         , asString);
  HA_SET_OBJECT_PROPERTY(manufacturerName , asString);
  HA_SET_OBJECT_PROPERTY(modelName        , asString);
  HA_SET_OBJECT_PROPERTY(description      , asString);
  HA_SET_OBJECT_PROPERTY(interviewFinished, asBool  );
  HA_SET_OBJECT_PROPERTY(supported        , asBool  );
  HA_SET_OBJECT_PROPERTY(active           , asBool  );
  HA_SET_OBJECT_PROPERTY(cloud            , asBool  );
  HA_SET_OBJECT_PROPERTY(discovery        , asBool  );
}

void CEndpoint::updateExpose(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  HA_LOG_DBG_INCOMING(payload.toStyledString());
  // HA_LOG_DBG("Items: " << ["items"].toStyledString());

  for(const Json::Value &item : payload["items"])
  {
    const std::string itemName = item.asString();
    if (payload.isMember("options") && payload["options"].isMember(itemName))
    {
      if(payload["options"][itemName].isObject())
      {
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "select" &&
           payload["options"][itemName].isMember("enum"))
        {
          CProperty *property = properties()->ensure(itemName);
          property->readonly(false);
          property->valueType(EPropertyValueType::pvtEnum);
          ha::utils::CStrings enumList;
          for(const Json::Value &enumItem : payload["options"][itemName]["enum"])
          {
            enumList.push_back(enumItem.asString());
          }
          property->enumerate(enumList);
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "binary")
        {
          CProperty *property = properties()->ensure(itemName);
          property->readonly(true);
          property->valueType(EPropertyValueType::pvtBool);
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "toggle")
        {
          CProperty *property = properties()->ensure(itemName);
          property->readonly(false);
          property->valueType(EPropertyValueType::pvtBool);
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "sensor")
        {
          CProperty *property = properties()->ensure(itemName);
          property->readonly(true);
          if(payload["options"][itemName].isMember("enum"))
          {
            property->valueType(EPropertyValueType::pvtEnum);
            ha::utils::CStrings enumList;
            for(const Json::Value &enumItem : payload["options"][itemName]["enum"])
            {
              enumList.push_back(enumItem.asString());
            }
            property->enumerate(enumList);
          }
          else
          {
            property->valueType(EPropertyValueType::pvtDouble);
          }
          if(payload["options"][itemName].isMember("unit")) { property->unit(payload["options"][itemName]["unit"].asString()); }
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "number")
        {
          CProperty *property = properties()->ensure(itemName);
          property->readonly(false);
          property->valueType(EPropertyValueType::pvtDouble);
          if(payload["options"][itemName].isMember("min")) { property->min(payload["options"][itemName]["min"].asDouble()); }
          if(payload["options"][itemName].isMember("max")) { property->max(payload["options"][itemName]["max"].asDouble()); }
          property->step(payload["options"][itemName].isMember("step") ? payload["options"][itemName]["step"].asDouble() : 1);
          continue;
        }
      }
      else if(payload["options"][itemName].isString())
      {
        if(itemName == "switch" && payload["options"][itemName].asString() == "outlet")
        {
          CProperty *property = properties()->ensure("status");
          property->readonly(false);
          property->valueType(EPropertyValueType::pvtEnum);
          property->enumerate({"on", "off", "toggle"});
          continue;
        }
      }
      else if(payload["options"][itemName].isArray())
      {
        if(itemName == "light")
        {
          CProperty *property = properties()->ensure("status");
          property->readonly(false);
          property->valueType(EPropertyValueType::pvtEnum);
          property->enumerate({"on", "off", "toggle"});
        }
        for(const Json::Value &option : payload["options"][itemName])
        {
          if(option.isString())
          {
            const std::string opt = option.asString();
            CProperty *property = properties()->ensure(opt);
            property->readonly(false);
            if(opt == "color")
            {
              property->valueType(EPropertyValueType::pvtColor);
            }
            else
            {
              property->valueType(EPropertyValueType::pvtInt);
            }
          }
        }
      }
      // else if(payload["options"][itemName].isString())
      // {

      // }
    }
    else
    {
      if(itemName == "switch")
      {
        CProperty *property = properties()->ensure("status");
        property->readonly(false);
        property->valueType(EPropertyValueType::pvtEnum);
        property->enumerate({"on", "off", "toggle"});
        continue;
      }
      if(itemName == "irCode")
      {
        CProperty *property = properties()->ensure("learn");
        property->readonly(false);
        property->valueType(EPropertyValueType::pvtBool);
        property = properties()->ensure("irCode");
        property->readonly(false);
        property->valueType(EPropertyValueType::pvtString);
        continue;
      }
    }
  }
}

void CEndpoint::updateFd(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  ha::utils::CStrings updatedNames = findChangedProperties(payload);
  for(const std::string &key : updatedNames)
  {
    updateProperty(key, payload[key]);
  }
}

void CEndpoint::updateDevice(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  ha::utils::CStrings updatedNames = findChangedProperties(payload);
  for(const std::string &key : updatedNames)
  {
    updateProperty(key, payload[key]);
  }
}

void CEndpoint::updateProperty(const std::string &name, const Json::Value &item)
{
  if(properties()->exists(name))
  {
    HA_LOG_DBG_INCOMING("Updating property " << name);
    CProperty *property = properties()->get(name);
    switch(property->valueType())
    {
      case EPropertyValueType::pvtEnum  :
      case EPropertyValueType::pvtString: property->storage()->set(item.asString()); break;
      case EPropertyValueType::pvtBool  : property->storage()->set(item.asBool  ()); break;
      case EPropertyValueType::pvtDouble: property->storage()->set(item.asDouble()); break;
      case EPropertyValueType::pvtInt   : property->storage()->set(item.asInt   ()); break;
      case EPropertyValueType::pvtColor : property->storage()->set(CColor(item   )); break;
    }
  }
}

ha::utils::CStrings CEndpoint::findChangedProperties(const Json::Value &payload)
{
  ha::utils::CStrings updatedNames;
  ha::utils::CStrings allNames;
  for(const std::string &name : payload.getMemberNames())
  {
    CProperty *property = properties()->get(name);
    if(!property) { continue; }
    allNames.push_back(name);
    const Json::Value &item = payload[name];
    // if(property->storage()->empty() || property->last() != item) { updatedNames.push_back(name); }
    // HA_LOG_DBG("Searching property " << name);
    // CValue val = property->last();
    // HA_LOG_DBG("property val " << val.asString());
    if(property->storage()->empty())
    {
      updatedNames.push_back(name);
    }
    else
    {
      switch(property->valueType())
      {
        case EPropertyValueType::pvtEnum  :
        case EPropertyValueType::pvtString: if(property->last().asString() != item.asString()) { updatedNames.push_back(name); } break;
        case EPropertyValueType::pvtBool  : if(property->last().asBool  () != item.asBool  ()) { updatedNames.push_back(name); } break;
        case EPropertyValueType::pvtDouble: if(property->last().asDouble() != item.asDouble()) { updatedNames.push_back(name); } break;
        case EPropertyValueType::pvtInt   : if(property->last().asInt   () != item.asInt   ()) { updatedNames.push_back(name); } break;
        case EPropertyValueType::pvtColor : if(property->last().asColor () != CColor(item   )) { updatedNames.push_back(name); } break;
      }
    }
  }
  return updatedNames.empty() ? allNames : updatedNames;
}

}
}
