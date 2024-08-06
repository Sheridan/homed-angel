#pragma once
#include "mqtt/base/cmqttclient.h"
#include "mqtt/scripting/cmqttscriptcallback.h"
#include <mqtt/async_client.h>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <map>


namespace ha
{
namespace mqtt
{

class СMqttScriptClient : public СMqttClient
{
public:
  explicit СMqttScriptClient(const std::string& scriptName, const std::string& functionName, const std::string& path);
  virtual ~СMqttScriptClient();

private:
  std::string m_path;
  CMqttScriptCallback *m_callback;

};

}
}
