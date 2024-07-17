#include "mqtt/cmqttcallback.h"
#include "utils/string.h"
#include "mqtt/cpayload.h"
#include "mqtt/cdevicename.h"
#include "st.h"


namespace ha
{
namespace mqtt
{

CMqttCallback:: CMqttCallback() {}
CMqttCallback::~CMqttCallback() {}

void CMqttCallback::message_arrived(::mqtt::const_message_ptr msg)
{
  // HA_LOG_NFO(  "Message arrived:\n"
  //           << "\ttopic: '" << msg->get_topic() << "'\n"
  //           << "\tpayload: '" << msg->get_payload_str());
  CPayload *payload = new CPayload();
  if(payload->parse(msg->get_payload_str()))
  {
    #ifdef HA_DEBUG
    payload->dump();
    #endif // HA_DEBUG
    CTopic *topic = new CTopic(msg->get_topic());
    switch(topic->topicType())
    {
      case ETopic::tUnknown: break;
      case ETopic::tTd     : break;
      case ETopic::tStatus:
      {
        for(const Json::Value &device : payload->value()["devices"])
        {
          // HA_LOG_DBG();
          CTopic *statusTopic = new CTopic(topic, device["name"].asString());
          sendUpdate(statusTopic, device);
          delete statusTopic;
        }
      } break;
      case ETopic::tExpose:
      {
        for(const std::string &endpointKey : payload->value().getMemberNames())
        {
          HA_LOG_DBG("Expose key: " << endpointKey);
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

void CMqttCallback::sendUpdate(CTopic *topic, const Json::Value &payload)
{
  if(topic->device().empty()) { return; }
  switch(topic->serviceType())
  {
    case EService::sZigbee:
    case EService::sCustom:
    {
      HA_LOG_DBG("Incoming data from topic " << topic->topic());
      HA_ST.homed().update(topic, payload);
    } break;
  }
}

}
}
