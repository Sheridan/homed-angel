#include "mqtt/scripting/cmqttscriptcallback.h"
#include "utils/string.h"
#include "scripting/script/callers/ccallersingle.h"
#include "st.h"


namespace ha
{
namespace mqtt
{

CMqttScriptCallback:: CMqttScriptCallback(const std::string& scriptName, const std::string& functionName)
  : m_scriptName(scriptName),
    m_functionName(functionName)
{}
CMqttScriptCallback::~CMqttScriptCallback() {}

void CMqttScriptCallback::incomingMessage(const std::string &i_topic, const std::string &i_payload)
{
  HA_ST->angel()->manager()->script(m_scriptName)->mqttCaller()->append(m_functionName, {i_topic, i_payload});
}

}
}
