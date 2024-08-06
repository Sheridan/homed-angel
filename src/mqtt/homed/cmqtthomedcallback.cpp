#include "mqtt/homed/cmqtthomedcallback.h"
#include "utils/string.h"
#include "mqtt/homed/cpayload.h"
#include "mqtt/homed/cdevicename.h"
#include "st.h"


namespace ha
{
namespace mqtt
{

CMqttHomedCallback:: CMqttHomedCallback() {}
CMqttHomedCallback::~CMqttHomedCallback() {}

void CMqttHomedCallback::incomingMessage(const std::string &i_topic, const std::string &i_payload)
{
  CPayload *payload = new CPayload();
  if(payload->parse(i_payload))
  {
    #ifdef HA_DEBUG
    payload->dump();
    #endif // HA_DEBUG
    CTopic *topic = new CTopic(i_topic);
    switch(topic->topicType())
    {
      case ETopic::tUnknown: break;
      case ETopic::tTd     : break;
      case ETopic::tStatus:
      {
        for(const Json::Value &device : payload->value()["devices"])
        {
          CTopic *statusTopic = new CTopic(topic, device["name"].asString());
          sendUpdate(statusTopic, device);
          delete statusTopic;
        }
      } break;
      case ETopic::tExpose:
      {
        for(const std::string &endpointKey : payload->value().getMemberNames())
        {
          HA_LOG_DBG_INCOMING("Expose key: " << endpointKey);
          if(ha::utils::is_digit(endpointKey))
          {
            CTopic *exposeTopic = new CTopic(topic, topic->device(), endpointKey);
            sendUpdate(exposeTopic, payload->value()[endpointKey]);
            delete exposeTopic;
          }
          else
          {
            sendUpdate(topic, payload->value()[endpointKey]);
          }
        }
      } break;
      case ETopic::tDevice:
      {
        payload->replaceKey("status", "online");
        sendUpdate(topic, payload->value());
      } break;
      default:
      {
        sendUpdate(topic, payload->value());
      } break;
    }
    delete topic;
  }
  delete payload;
}


void CMqttHomedCallback::sendUpdate(CTopic *topic, const Json::Value &payload)
{
  if(topic->device().empty()) { return; }
  switch(topic->serviceType())
  {
    case EService::sZigbee:
    case EService::sCustom:
    {
      HA_LOG_DBG_INCOMING("Incoming data from topic " << topic->topic());
      HA_ST->homed()->update(topic, payload);
    } break;
  }
}

}
}
