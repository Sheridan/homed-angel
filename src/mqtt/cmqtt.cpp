#include "mqtt/cmqtt.h"
#include "cmqtt.h"

namespace ha
{
namespace mqtt
{


CMqtt::CMqtt()
{
  m_publisher = new СMqttPublisher();
  m_clientHomed = new СMqttHomedClient();
  m_publisher->connect();
}

CMqtt::~CMqtt()
{
  delete m_clientHomed;
  delete m_publisher;
}

void CMqtt::subscribe(const std::string &scriptName, const std::string &functionName, const std::string &path)
{
  СMqttScriptClient *client;
  if(m_clientsScript.contains(scriptName) && m_clientsScript[scriptName].contains(functionName))
  {
    client = m_clientsScript[scriptName][functionName];
    if(client->subscrptionTopic() != path)
    {
      client->unsubscribe();
      client->subscribe(path);
    }
  }
  else
  {
    client = new СMqttScriptClient(scriptName, functionName, path);
    client->connect();
    client->subscribe();
    m_clientsScript[scriptName][functionName] = client;
  }
}

void CMqtt::publish(const std::string &topic, const std::string &content)
{
  publisher()->publish(topic, content);
}

void CMqtt::unsubscribe(const std::string &scriptName)
{
  if(m_clientsScript.contains(scriptName))
  {
    for (auto it = m_clientsScript[scriptName].begin(); it != m_clientsScript[scriptName].end(); )
    {
      СMqttScriptClient *client = it->second;
      it = m_clientsScript[scriptName].erase(it);
      delete client;
    }
  }
}

}
}
