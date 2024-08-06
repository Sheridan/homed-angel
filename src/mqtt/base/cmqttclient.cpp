#include "mqtt/base/cmqttclient.h"
#include <iostream>
#include "st.h"
#include "cmqttclient.h"

namespace ha
{
namespace mqtt
{

СMqttClient::СMqttClient(const std::string &topic)
    : СMqttBase("client"),
      m_callback(nullptr),
      m_subscrptionTopic(topic)
{}

СMqttClient::~СMqttClient()
{
  unsubscribe();
}

void СMqttClient::connect()
{
  СMqttBase::connect();
  client()->set_callback(*m_callback);
}

void СMqttClient::subscribe()
{
  try
  {
    client()->subscribe(m_subscrptionTopic, 1)->wait();
    HA_LOG_NFO("Subscribed to topic '" << m_subscrptionTopic << "'.");
  }
  catch (const ::mqtt::exception& e)
  {
    HA_ABORT("MQTT subscription error: " << e.what());
  }
}

void СMqttClient::subscribe(const std::string &topic)
{
  m_subscrptionTopic = topic;
  subscribe();
}

void СMqttClient::unsubscribe()
{
  try
  {
    client()->unsubscribe(m_subscrptionTopic)->wait();
  }
  catch (const ::mqtt::exception& e)
  {
    HA_LOG_ERR("Error during MQTT unsubscribe: " << e.what());
  }
}

void СMqttClient::setCallback(CMqttCallback *callback)
{
  m_callback = callback;
}

}
}
