#pragma once
#include <string>

namespace ha
{
namespace homed
{

class CProperty;
class CObserver
{
public:
  CObserver(const std::string &sn, const std::string &mn, const bool &changedOnly, CProperty *pProperty);
  ~CObserver();
  const std::string &scriptName() const { return m_scriptName; }
  const std::string &methodName() const { return m_methodName; }
  void call();
private:
  std::string m_scriptName;
  std::string m_methodName;
  bool m_changedOnly;
  CProperty *m_parentProperty;
};



}
}
