#pragma once
#include <queue>
#include <string>
#include <thread>
#include "scripting/script/cscriptenvironment.h"
#include "homed/model/entities/cproperty.h"

namespace ha
{
namespace scripting
{

struct SPropertyUpdate
{
  std::string method;
  ha::homed::CProperty *property;
  SPropertyUpdate(const std::string &m, ha::homed::CProperty *p) : method(m), property(p) {};
};

struct STimerShoot
{
  std::string method;
  explicit STimerShoot(const std::string &m) : method(m) {};
};

class CScript : public CScriptEnvironment
{
public:
  explicit CScript(const std::filesystem::path &file);
  virtual ~CScript();

  CScript(const CScript&) = delete;
  CScript& operator=(const CScript&) = delete;

  void run();
  void queuePropertyChanged(const std::string &method, ha::homed::CProperty *property);
  void queueSimpleFunctionCall(const std::string &method);

private:
  bool m_running;
  std::thread m_thread;
  std::queue<SPropertyUpdate> m_propertyUpdates;
  std::queue<STimerShoot> m_timerShoots;
  void callMethod(const std::string &method);
  void callPropertyChanged(const SPropertyUpdate &propertyUpdate);
  void callTimerShoot(const STimerShoot &timerShoot);

};




}
}
