#include "mqtt/cpayload.h"
#include "st.h"

namespace ha
{
namespace mqtt
{

CPayload::CPayload()
{
  m_builder = new Json::CharReaderBuilder();
  m_reader = m_builder->newCharReader();
}

CPayload::~CPayload()
{
  delete m_builder;
  delete m_reader;
}

bool CPayload::validate(const std::string& payload)
{
  if(payload.empty()) { return false; }
  return true;
}

bool CPayload::parse(const std::string& payload)
{
  HA_LOG_DBG_MQTT(payload);
  if(!validate(payload)) { return false; }
  JSONCPP_STRING err;
  if (!m_reader->parse(payload.c_str(), payload.c_str() + payload.length(), &m_root, &err))
  {
    HA_LOG_WRN("MQTT payload parse error: " << err);
    return false;
  }
  HA_LOG_DBG_MQTT(m_root.toStyledString());
  return true;
}

void CPayload::replaceKey(const std::string &key, const std::string &replacement)
{
  if(m_root.isMember(key))
  {
    m_root[replacement] = m_root[key];
    m_root.removeMember(key);
  }
  #ifdef HA_DEBUG
  dump();
  #endif // HA_DEBUG
}

#ifdef HA_DEBUG
void CPayload::dump()
{
  HA_LOG_DBG_MQTT(m_root);
}
#endif



}
}
