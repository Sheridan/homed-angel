#pragma once
#include <mqtt/async_client.h>
#include "mqtt/base/cmqttcallback.h"

namespace ha
{
namespace mqtt
{

struct SMqttMesssage
{
  std::string topic;
  std::string payload;
};

class CMqttScriptCallback : public CMqttCallback
{
public:
  explicit CMqttScriptCallback(const std::string& scriptName, const std::string& functionName);
  virtual ~CMqttScriptCallback();

private:
  void incomingMessage(const std::string &i_topic, const std::string &i_payload) final;
  std::string m_scriptName;
  std::string m_functionName;
};

}
}
