#pragma once
#include "mqtt/base/cmqttbase.h"
#include "mqtt/base/cmqttcallback.h"
#include <string>
#include <queue>
#include <thread>
#include <mutex>


namespace ha
{
namespace mqtt
{

class СMqttPublisher : public СMqttBase
{
public:
  explicit СMqttPublisher();
  virtual ~СMqttPublisher();

  void connect();
  void publish(const std::string &topic, const std::string &content);


private:
  std::queue<std::pair<std::string, std::string>> m_publishMessages;
  std::mutex m_publishMutex;
  std::thread m_publishThread;
  bool m_publishStop = false;
  int m_publishInterval;

  void publishWorker();
};

}
}
