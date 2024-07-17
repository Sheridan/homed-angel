#pragma once
#include "mqtt/cmqttcallback.h"
#include <mqtt/async_client.h>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
// #include <chrono>
// #include <condition_variable>

namespace ha
{
namespace mqtt
{

using TMqttClient = ::mqtt::async_client;
using TMqttConnectOptions = ::mqtt::connect_options;

class СMqttClient
{
public:
  СMqttClient();
  ~СMqttClient();

  void connect(const std::string& serverAddress, const std::string& clientId,
               const std::string& username     , const std::string& password);
  void subscribe();
  void publish(const std::string &topic, const std::string &content);

private:
  TMqttClient *m_client;
  TMqttConnectOptions *m_connectionOptions;
  CMqttCallback m_callback;

  std::queue<std::pair<std::string, std::string>> m_publishMessages;
  std::mutex m_publishMutex;
  // std::condition_variable m_publishCondition;
  std::thread m_publishThread;
  bool m_publishStop = false;
  int m_publishInterval;

  void publishWorker();
};

}
}
