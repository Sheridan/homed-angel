#pragma once
#include <mqtt/async_client.h>
#include "mqtt/base/cmqttcallback.h"
#include <string>
#include <queue>
#include <thread>
#include <mutex>


namespace ha
{
namespace mqtt
{

using TMqttClient = ::mqtt::async_client;
using TMqttConnectOptions = ::mqtt::connect_options;

class СMqttBase
{
public:
  explicit СMqttBase(const std::string &clientIdSuffix);
  virtual ~СMqttBase();

  void connect();
  void disconnect();

protected:
  TMqttClient *client() const { return m_client; }

private:
  TMqttClient *m_client;
  TMqttConnectOptions *m_connectionOptions;
  std::string m_serverAddress;
  std::string m_clientIdSuffix;
  std::string m_username;
  std::string m_password;

  std::string clientID();
};

}
}
