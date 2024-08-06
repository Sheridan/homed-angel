#pragma once
#include <mqtt/async_client.h>

namespace ha
{
namespace mqtt
{

class CMqttCallback : public virtual ::mqtt::callback
{
public:
  CMqttCallback();
  virtual ~CMqttCallback();
  virtual void incomingMessage(const std::string &i_topic, const std::string &i_payload) = 0;

private:
  void message_arrived(::mqtt::const_message_ptr msg) override;
};

}
}
