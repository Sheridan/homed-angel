#pragma once
#include "mqtt/base/cmqttcallback.h"
#include <jsoncpp/json/json.h>
#include "mqtt/homed/ctopic.h"

namespace ha
{
namespace mqtt
{

class CMqttHomedCallback : public CMqttCallback
{
public:
  CMqttHomedCallback();
  virtual ~CMqttHomedCallback();

private:
  void incomingMessage(const std::string &i_topic, const std::string &i_payload) final;
  void sendUpdate(CTopic *topic, const Json::Value &payload);
};

}
}
