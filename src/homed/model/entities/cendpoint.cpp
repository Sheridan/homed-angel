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

CProperty* CEndpoint::addProperty(const std::string &name, const bool &readonly, const EPropertyValueType &type)
{
  CProperty *property = properties()->ensure(name);
  property->readonly(readonly);
  property->valueType(type);
  return property;
}

CProperty* CEndpoint::addEnumerateToProperty(CProperty* property, const ha::utils::CStrings &enums = {})
{
  property->enumerate(enums);
  return property;
}

CProperty* CEndpoint::addEnumerateToProperty(CProperty* property, const Json::Value &enums = {})
{
  ha::utils::CStrings enumList;
  for(const Json::Value &enumItem : enums)
  {
    enumList.push_back(enumItem.asString());
  }
  return addEnumerateToProperty(property, enumList);
}

CProperty* CEndpoint::addBordersToProperty(CProperty* property, const double &min, const double &max, const double &step)
{
  property->min(min);
  property->max(max);
  property->step(step);
  return property;
}

CProperty* CEndpoint::addBordersToProperty(CProperty* property, const Json::Value &borders)
{
  if(borders.isMember("min")) { property->min(borders["min"].asDouble()); }
  if(borders.isMember("max")) { property->max(borders["max"].asDouble()); }
  property->step(borders.isMember("step") ? borders["step"].asDouble() : 1);
  return property;
}

CProperty* CEndpoint::addUnitToProperty(CProperty* property, const std::string &unit)
{
  property->unit(unit);
  return property;
}

#define HA_SET_OBJECT_PROPERTY(_name, _as) if (payload.isMember(#_name)) { device->_name(payload[#_name]._as()); }
void CEndpoint::updateStatus(const ha::mqtt::CTopic *topic, const Json::Value &payload)
{
  HA_LOG_DBG_INCOMING("Updating status: " << topic->device());
                         addProperty("linkQuality", true, EPropertyValueType::pvtInt );
                         addProperty("lastSeen"   , true, EPropertyValueType::pvtInt );
  addEnumerateToProperty(addProperty("online"     , true, EPropertyValueType::pvtEnum), ha::utils::CStrings {"online", "offline"});

  CDevice *device = HA_ST->homed()->instance(topic->instance())->devices(topic->serviceType())->ensure(topic->device());
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
          addEnumerateToProperty(addProperty(itemName, false, EPropertyValueType::pvtEnum), payload["options"][itemName]["enum"]);
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "binary")
        {
          addProperty(itemName, true, EPropertyValueType::pvtBool);
          continue;
        }
        if(payload["options"][itemName].isMember("type") &&
            (
              payload["options"][itemName]["type"].asString() == "toggle" ||
              payload["options"][itemName]["type"].asString() == "button"
            )
          )
        {
          addProperty(itemName, false, EPropertyValueType::pvtBool);
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "sensor")
        {
          CProperty *property = nullptr;
          if(payload["options"][itemName].isMember("enum"))
          {
            property = addEnumerateToProperty(addProperty(itemName, true, EPropertyValueType::pvtEnum), payload["options"][itemName]["enum"]);
          }
          else
          {
            property = addProperty(itemName, true, EPropertyValueType::pvtDouble);
          }
          if(payload["options"][itemName].isMember("unit")) { addUnitToProperty(property, payload["options"][itemName]["unit"].asString()); }
          continue;
        }
        if(payload["options"][itemName].isMember("type") && payload["options"][itemName]["type"].asString() == "number")
        {
          addBordersToProperty(addProperty(itemName, false, EPropertyValueType::pvtDouble), payload["options"][itemName]);
          continue;
        }
      }
      else if(payload["options"][itemName].isString())
      {
        if(itemName == "switch" && payload["options"][itemName].asString() == "outlet")
        {
          addEnumerateToProperty(addProperty("status", false, EPropertyValueType::pvtEnum), ha::utils::CStrings {"on", "off", "toggle"});
          continue;
        }
      }
      else if(payload["options"][itemName].isArray())
      {
        if(itemName == "light")
        {
          addEnumerateToProperty(addProperty("status", false, EPropertyValueType::pvtEnum), ha::utils::CStrings {"on", "off", "toggle"});
        }
        for(const Json::Value &option : payload["options"][itemName])
        {
          if(option.isString())
          {
            const std::string opt = option.asString();
            if(opt == "color")
            {
              addProperty(opt, false, EPropertyValueType::pvtColor);
            }
            else
            {
              addProperty(opt, false, EPropertyValueType::pvtInt);
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
        addEnumerateToProperty(addProperty("status", false, EPropertyValueType::pvtEnum), ha::utils::CStrings {"on", "off", "toggle"});
        continue;
      }
      if(itemName == "irCode")
      {
        addProperty("learn", false, EPropertyValueType::pvtBool);
        addProperty("irCode", false, EPropertyValueType::pvtString);
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

std::string CEndpoint::topicPath(const ha::mqtt::ETopic &topic)
{
  std::string path = m_parentDevice->topicPath(topic);
  switch(topic)
  {
    case ha::mqtt::ETopic::tFd:
    case ha::mqtt::ETopic::tTd:
    {
      path += "/" + name();
    } break;
  }
  return path;
}

}
}
