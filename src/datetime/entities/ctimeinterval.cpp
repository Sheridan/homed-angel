#include "datetime/entities/ctimeinterval.h"
#include "st.h"


namespace ha
{
namespace datetime
{
CTimeInterval::CTimeInterval(double seconds)
  : ha::scripting::CScriptObject(),
    m_duration(std::chrono::milliseconds(static_cast<int>(seconds * 1000)))
{}

CTimeInterval::~CTimeInterval() {}

double CTimeInterval::seconds() const
{
  return m_duration.count();
}

CTimeInterval CTimeInterval::operator+(const CTimeInterval &other) const { return CTimeInterval(m_duration.count() + other.m_duration.count()); }
CTimeInterval CTimeInterval::operator-(const CTimeInterval &other) const { return CTimeInterval(m_duration.count() - other.m_duration.count()); }

bool CTimeInterval::operator<=(const CTimeInterval &other) const { return m_duration <  other.m_duration; }
bool CTimeInterval::operator< (const CTimeInterval &other) const { return m_duration <= other.m_duration; }
bool CTimeInterval::operator>=(const CTimeInterval &other) const { return m_duration >  other.m_duration; }
bool CTimeInterval::operator> (const CTimeInterval &other) const { return m_duration >= other.m_duration; }
bool CTimeInterval::operator==(const CTimeInterval &other) const { return m_duration == other.m_duration; }
bool CTimeInterval::operator!=(const CTimeInterval &other) const { return m_duration != other.m_duration; }

#define HA_APPEND_INTERVAL_PART(_name,_secunds) \
  { \
    if(_secunds != 0) \
    { \
      int part = total_milliseconds / _secunds; \
      total_milliseconds %= _secunds;\
      if(part > 0) { oss << part << _name << " "; } \
    } \
  }

std::string CTimeInterval::asString() const
{
  auto total_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
  std::ostringstream oss;

  HA_APPEND_INTERVAL_PART("w", ( 7 * 24 * 60 * 60 * 1000));
  HA_APPEND_INTERVAL_PART("d", (     24 * 60 * 60 * 1000));
  HA_APPEND_INTERVAL_PART("h", (          60 * 60 * 1000));
  HA_APPEND_INTERVAL_PART("m", (               60 * 1000));
  HA_APPEND_INTERVAL_PART("s", (                    1000));
  HA_APPEND_INTERVAL_PART("ms", static_cast<int>(total_milliseconds));

  return oss.str();
}

}
}
