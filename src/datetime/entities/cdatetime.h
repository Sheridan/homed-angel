#pragma once
#include <chrono>
#include <string>
#include "datetime/entities/ctimeinterval.h"

namespace ha
{
namespace datetime
{

class CDateTime
{
public:
  explicit CDateTime();
  explicit CDateTime(std::time_t timestamp);
  explicit CDateTime(const std::chrono::steady_clock::time_point& steadyTimePoint);
  explicit CDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond);
           CDateTime(const CDateTime& other);
  virtual ~CDateTime();

  std::string asString(const std::string& format = "%Y.%m.%d %H:%M:%S") const;
  std::time_t asUnixTimestamp() const;

  CDateTime& operator=(const CDateTime& other);
  CDateTime  operator+(const CTimeInterval& interval) const;
  CDateTime  operator-(const CTimeInterval& interval) const;

  bool operator< (const CDateTime& other) const;
  bool operator<=(const CDateTime& other) const;
  bool operator> (const CDateTime& other) const;
  bool operator>=(const CDateTime& other) const;
  bool operator==(const CDateTime& other) const;
  bool operator!=(const CDateTime& other) const;

private:
  std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> m_time;
};

}
}
