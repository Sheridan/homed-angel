#include "datetime/entities/cdatetime.h"
#include "st.h"
#include "cdatetime.h"

namespace ha
{
namespace datetime
{

CDateTime::CDateTime()
{
  auto now = std::chrono::system_clock::now();
  m_time = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
}

CDateTime::CDateTime(std::time_t timestamp)
{
  m_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(
  std::chrono::milliseconds(timestamp * 1000));
}

CDateTime::CDateTime(const std::string &str)
{
  std::tm tm = {};
  std::istringstream ss(str);

  if (str.find(' ') != std::string::npos)
  {
    ss >> std::get_time(&tm, "%Y.%m.%d %H:%M:%S");
  }
  else if (str.find('.') != std::string::npos)
  {
    ss >> std::get_time(&tm, "%Y.%m.%d");
  }
  else
  {
    ss >> std::get_time(&tm, "%H:%M:%S");
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time_t);
    tm.tm_year = now_tm->tm_year;
    tm.tm_mon  = now_tm->tm_mon ;
    tm.tm_mday = now_tm->tm_mday;

    if (tm.tm_hour <  now_tm->tm_hour ||
       (tm.tm_hour == now_tm->tm_hour && tm.tm_min <  now_tm->tm_min) ||
       (tm.tm_hour == now_tm->tm_hour && tm.tm_min == now_tm->tm_min && tm.tm_sec < now_tm->tm_sec))
    {
      tm.tm_mday += 1;
    }
  }

  if (ss.fail())
  {
    HA_LOG_ERR("'" << str << "' is wrong datetime format");
  }
  else
  {
    auto time_t = std::mktime(&tm);
    m_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(std::chrono::milliseconds(time_t * 1000));
    // m_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(time_t) + std::chrono::milliseconds(tm.tm_sec * 1000));
  }
}

CDateTime::CDateTime(const std::chrono::steady_clock::time_point &steadyTimePoint)
{
  auto duration = steadyTimePoint.time_since_epoch();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  m_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(milliseconds);
}

CDateTime::CDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
  std::tm tm = {};
  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = minute;
  tm.tm_sec = second;
  auto time_t = std::mktime(&tm);
  m_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(std::chrono::milliseconds(time_t * 1000 + millisecond));
}

CDateTime::CDateTime(const CDateTime& other)
  : m_time(other.m_time)
{}

CDateTime::~CDateTime()
{
  // HA_LOG_DBG("CDateTime::~CDateTime");
}

std::string CDateTime::asString(const std::string &format) const
{
  std::time_t time_t = std::chrono::system_clock::to_time_t(m_time);
  std::tm tm = *std::localtime(&time_t);

  std::ostringstream oss;
  oss << std::put_time(&tm, format.c_str());

  return oss.str();
}

std::string CDateTime::asString() const
{
  return asString("%Y.%m.%d %H:%M:%S");
}

std::time_t CDateTime::asUnixTimestamp() const
{
  return std::chrono::duration_cast<std::chrono::seconds>(m_time.time_since_epoch()).count();
}

CDateTime& CDateTime::operator=(const CDateTime& other)
{
  if (this != &other)
  {
    m_time = other.m_time;
  }
  return *this;
}

CDateTime CDateTime::operator+(const CTimeInterval &interval) const { return CDateTime(std::chrono::duration_cast<std::chrono::seconds>(m_time.time_since_epoch()).count() + interval.seconds()); }
CDateTime CDateTime::operator-(const CTimeInterval &interval) const { return CDateTime(std::chrono::duration_cast<std::chrono::seconds>(m_time.time_since_epoch()).count() - interval.seconds()); }

bool CDateTime::operator<=(const CDateTime& other) const { return m_time <  other.m_time; }
bool CDateTime::operator< (const CDateTime& other) const { return m_time <= other.m_time; }
bool CDateTime::operator>=(const CDateTime& other) const { return m_time >  other.m_time; }
bool CDateTime::operator> (const CDateTime& other) const { return m_time >= other.m_time; }
bool CDateTime::operator==(const CDateTime& other) const { return m_time == other.m_time; }
bool CDateTime::operator!=(const CDateTime& other) const { return m_time != other.m_time; }


}
}
