#include "mqtt/base/cmqttbase.h"
#include <iostream>
#include "st.h"
#include "utils/string.h"
#include "cmqttbase.h"

namespace ha
{
namespace mqtt
{

СMqttBase::СMqttBase(const std::string &clientIdSuffix)
    : m_client(nullptr),
      m_connectionOptions(nullptr),
      m_serverAddress("tcp://" + HA_ST->config()->mqttServer() + ":" + std::to_string(HA_ST->config()->mqttPort())),
      m_clientIdSuffix(clientIdSuffix),
      m_username(HA_ST->config()->mqttUser()),
      m_password(HA_ST->config()->mqttPassword())
{}

СMqttBase::~СMqttBase()
{
  disconnect();
}

void СMqttBase::connect()
{
  m_client = new TMqttClient(m_serverAddress, clientID());
  m_connectionOptions = new TMqttConnectOptions();
  m_connectionOptions->set_user_name(m_username);
  m_connectionOptions->set_password(m_password);

  try
  {
    HA_LOG_VERBOSE("Connecting to the MQTT server...");
    m_client->connect(*m_connectionOptions)->wait();
    HA_LOG_VERBOSE("MQTT connected");
  }
  catch (const ::mqtt::exception& e)
  {
    HA_ABORT("MQTT connection error: " << e.what());
  }
}

void СMqttBase::disconnect()
{
  try
  {
    HA_LOG_VERBOSE("Disonnecting from the MQTT server...");
    m_client->disconnect()->wait();
    HA_LOG_VERBOSE("MQTT disconnected");
  }
  catch (const ::mqtt::exception& e)
  {
    HA_LOG_ERR("Error during MQTT disconnection: " << e.what());
  }
  delete m_client;
  delete m_connectionOptions;
}

std::string СMqttBase::clientID()
{
  return "homed-angel|" + m_clientIdSuffix + "|" + ha::utils::random(8);
}
}
}
