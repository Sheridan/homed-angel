#pragma once
#include <chrono>
#include <string>


namespace ha
{
namespace datetime
{

class CTimeInterval
{
public:
  explicit CTimeInterval(double seconds);
  virtual ~CTimeInterval();

  double seconds() const;
  std::string asString() const;

  CTimeInterval operator+(const CTimeInterval& other) const;
  CTimeInterval operator-(const CTimeInterval& other) const;

  bool operator< (const CTimeInterval& other) const;
  bool operator<=(const CTimeInterval& other) const;
  bool operator>=(const CTimeInterval& other) const;
  bool operator> (const CTimeInterval& other) const;
  bool operator==(const CTimeInterval& other) const;
  bool operator!=(const CTimeInterval& other) const;

private:
  std::chrono::milliseconds m_duration;
};

}
}
