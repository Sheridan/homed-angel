#include "mqtt/cmqttclient.h"
#include <iostream>
#include "st.h"

namespace ha
{
namespace mqtt
{

СMqttClient::СMqttClient()
    : m_client(nullptr),
      m_connectionOptions(nullptr),
      m_publishInterval(500)
{}

СMqttClient::~СMqttClient()
{
  m_publishStop = true;
  m_publishThread.join();
  try
  {
    m_client->disconnect()->wait();
    delete m_client;
    delete m_connectionOptions;
  }
  catch (const ::mqtt::exception& e)
  {
    HA_LOG_ERR("Error during MQTT disconnection: " << e.what());
  }
}

void СMqttClient::connect(const std::string &serverAddress, const std::string &clientId, const std::string &username, const std::string &password)
{
  m_client = new TMqttClient(serverAddress, clientId);
  m_connectionOptions = new TMqttConnectOptions();
  m_connectionOptions->set_user_name(username);
  m_connectionOptions->set_password(password);

  m_client->set_callback(m_callback);

  try
  {
    HA_LOG_NFO("Connecting to the MQTT server...");
    m_client->connect(*m_connectionOptions)->wait();
    HA_LOG_NFO("MQTT connected.");
    m_publishThread = std::thread(&СMqttClient::publishWorker, this);
  }
  catch (const ::mqtt::exception& e)
  {
    HA_ABORT("MQTT connection error: " << e.what());
  }
}

void СMqttClient::subscribe()
{
  try
  {
    std::string topic = HA_ST->config()->mqttHomedTopic() + "/#";
    m_client->subscribe(topic, 1)->wait();
    HA_LOG_NFO("Subscribed to topic '" << topic << "'.");
  }
  catch (const ::mqtt::exception& e)
  {
    HA_ABORT("MQTT subscription error: " << e.what());
  }
}

void СMqttClient::publish(const std::string &topic, const std::string &content)
{
  std::lock_guard<std::mutex> lock(m_publishMutex);
  m_publishMessages.emplace(topic, content);
}

void СMqttClient::publishWorker()
{
  m_publishStop = false;
  while (!m_publishStop)
  {
    std::unique_lock<std::mutex> lock(m_publishMutex);
    bool qEmpty = m_publishMessages.empty();
    lock.unlock();
    if(qEmpty) { HA_ST->sleep(HA_DEFAULT_SLEEP_MS/2); }
    else
    {
      std::unique_lock<std::mutex> lock(m_publishMutex);
      auto [topic, content] = m_publishMessages.front();
      m_publishMessages.pop();
      lock.unlock();
      try
      {
        HA_LOG_DBG_MQTT("Publishing '" << content << "'" << " to topic " << topic);
        auto msg = ::mqtt::make_message(topic, content);
        msg->set_qos(1);
        m_client->publish(msg)->wait();
      }
      catch (const ::mqtt::exception& e)
      {
        HA_LOG_ERR("Error sending message: " << e.what());
      }
      HA_ST->sleep(m_publishInterval);
    }
  }
}
}
}
