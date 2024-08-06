#include "mqtt/homed/cmqtthomedclient.h"
#include <iostream>
#include "st.h"

namespace ha
{
namespace mqtt
{

СMqttHomedClient::СMqttHomedClient()
  : СMqttClient(HA_ST->config()->mqttHomedTopic() + "/#")
{
  m_callback = new CMqttHomedCallback();
  setCallback(m_callback);
}

СMqttHomedClient::~СMqttHomedClient()
{
  delete m_callback;
}

}
}
