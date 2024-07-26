#pragma once
#include <iostream>
#include <string>
#include <map>

namespace ha
{
namespace scripting
{
namespace helpers
{

enum ELogPriority
{
  lpDebug    = 0,
  lpInfo     = 1,
  lpWarning  = 2,
  lpError    = 3,
  lpCritical = 4
};

#define HA_SLOG_METHOD(_name,_priority) void _name(const std::string &message) { log(ELogPriority::_priority, message); }

class CLogger
{

public:
  explicit CLogger(const std::string &name);
  virtual ~CLogger();

  HA_SLOG_METHOD(dbg, lpDebug   );
  HA_SLOG_METHOD(nfo, lpInfo    );
  HA_SLOG_METHOD(wrn, lpWarning );
  HA_SLOG_METHOD(err, lpError   );
  HA_SLOG_METHOD(cry, lpCritical);

private:
  std::string m_name;
  void log(const ELogPriority &pri, const std::string &message);

  const char m_priorities[5][4] = { "dbg", "inf", "wrn", "err", "cri" };
  void *m_outs[5] = { &(std::cout), &(std::cout), &(std::cerr), &(std::cerr), &(std::cerr) };

};

}
}
}
