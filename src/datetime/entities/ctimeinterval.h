#pragma once
#include <chrono>
#include <string>
#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace datetime
{

class CTimeInterval : public ha::scripting::CScriptObject
{
public:
  explicit CTimeInterval(double seconds);
  explicit CTimeInterval(const std::chrono::milliseconds &seconds);
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
