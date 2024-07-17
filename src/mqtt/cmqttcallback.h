#pragma once
#include <mqtt/async_client.h>
#include <jsoncpp/json/json.h>
#include "mqtt/ctopic.h"

namespace ha
{
namespace mqtt
{


class CMqttCallback : public virtual ::mqtt::callback
{
public:
  CMqttCallback();
  ~CMqttCallback();
  void message_arrived(::mqtt::const_message_ptr msg) override;

private:
  void sendUpdate(CTopic *topic, const Json::Value &payload);
};

}
}
