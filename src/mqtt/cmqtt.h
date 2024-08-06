#pragma once
#include <string>
#include <map>
#include "mqtt/homed/cmqtthomedclient.h"
#include "mqtt/scripting/cmqttscriptclient.h"
#include "mqtt/base/cmqttpublisher.h"

namespace ha
{
namespace mqtt
{

class CMqtt
{
public:
  CMqtt();
  ~CMqtt();

  СMqttPublisher *publisher() const { return m_publisher; }
  СMqttHomedClient *clientHomed() const { return m_clientHomed; }
  void unsubscribe(const std::string& scriptName);

  void subscribe(const std::string& scriptName, const std::string& functionName, const std::string& path);
  void publish(const std::string &topic, const std::string &content);

private:
  СMqttPublisher *m_publisher;
  СMqttHomedClient *m_clientHomed;
  std::map<std::string, std::map<std::string, СMqttScriptClient *>> m_clientsScript;

};



}
}
