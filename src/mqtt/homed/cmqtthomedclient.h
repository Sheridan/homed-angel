#pragma once
#include "mqtt/base/cmqttclient.h"
#include "mqtt/homed/cmqtthomedcallback.h"
#include <mqtt/async_client.h>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <map>


namespace ha
{
namespace mqtt
{

class СMqttHomedClient : public СMqttClient
{
public:
  СMqttHomedClient();
  ~СMqttHomedClient();

private:
  CMqttHomedCallback *m_callback;

};

}
}
