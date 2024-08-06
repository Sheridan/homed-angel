#include "mqtt/base/cmqttcallback.h"
#include "utils/string.h"
#include "st.h"


namespace ha
{
namespace mqtt
{

CMqttCallback:: CMqttCallback() {}
CMqttCallback::~CMqttCallback() {}

void CMqttCallback::message_arrived(::mqtt::const_message_ptr msg)
{
  // HA_LOG_NFO(  "Message arrived:\n"
  //           << "\ttopic: '" << msg->get_topic() << "'\n"
  //           << "\tpayload: '" << msg->get_payload_str());

  incomingMessage(msg->get_topic(), msg->get_payload_str());
}

}
}
