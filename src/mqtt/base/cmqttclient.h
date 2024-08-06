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

class СMqttClient : public СMqttBase
{
  friend class CMqttCallback;
public:
  explicit СMqttClient(const std::string &topic);
  virtual ~СMqttClient();

  void connect();
  void subscribe();
  void subscribe(const std::string &topic);
  void unsubscribe();
  const std::string &subscrptionTopic() const { return m_subscrptionTopic; }

protected:
  void setCallback(CMqttCallback *callback);

private:
  CMqttCallback *m_callback;
  std::string m_subscrptionTopic;
};

}
}
