#include "mqtt/scripting/cmqttscriptclient.h"
#include "st.h"

namespace ha
{
namespace mqtt
{

СMqttScriptClient::СMqttScriptClient(const std::string& scriptName, const std::string& functionName, const std::string& path)
  : СMqttClient(path)
{
  m_callback = new CMqttScriptCallback(scriptName, functionName);
  setCallback(m_callback);
}

СMqttScriptClient::~СMqttScriptClient()
{
  delete m_callback;
}

}
}
