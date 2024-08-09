#include "mqtt/base/cmqttpublisher.h"
#include <iostream>
#include "utils/thread.h"
#include "st.h"

namespace ha
{
namespace mqtt
{

СMqttPublisher::СMqttPublisher()
    : СMqttBase("publisher"),
      m_publishInterval(HA_ST->config()->mqttPublishInterval())
{}

СMqttPublisher::~СMqttPublisher()
{
  m_publishStop = true;
  m_publishThread.join();
}

void СMqttPublisher::connect()
{
  СMqttBase::connect();
  m_publishThread = std::thread(&СMqttPublisher::publishWorker, this);
}

void СMqttPublisher::publish(const std::string &topic, const std::string &content)
{
  std::lock_guard<std::mutex> lock(m_publishMutex);
  m_publishMessages.emplace(topic, content);
}

void СMqttPublisher::publishWorker()
{
  ha::utils::set_thread_name("MQTT Publisher");
  m_publishStop = false;
  while (!m_publishStop)
  {
    std::unique_lock<std::mutex> lock(m_publishMutex);
    bool qEmpty = m_publishMessages.empty();
    lock.unlock();
    if(qEmpty) { ha::utils::sleep(10); }
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
        msg->set_qos(HA_ST->config()->mqttPublishQOS());
        client()->publish(msg)->wait_for(m_publishInterval);
      }
      catch (const ::mqtt::exception& e)
      {
        HA_LOG_ERR("Error sending message: " << e.what());
      }
      ha::utils::sleep(m_publishInterval);
    }
  }
}
}
}
