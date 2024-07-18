#pragma once
#include "datetime/timers/ctimer.h"
#include "scripting/script/cscriptobject.h"

namespace ha
{
namespace datetime
{

class CTimerOneshot : public CTimer, public ha::scripting::CScriptObject
{
public:
  explicit CTimerOneshot(const std::string &scriptName, const std::string &functionName, const std::chrono::milliseconds &interval);
  virtual ~CTimerOneshot();

  CTimerOneshot &operator=(const CTimerOneshot &other);

protected:
  void run() override;
};

}
}
