#pragma once
#include "homed/value/cvalue.h"
#include "homed/value/cobserver.h"
#include "log.h"
#include <vector>
#include <deque>
#include <mutex>
#include <shared_mutex>
// #include <memory>

namespace ha
{
namespace homed
{

using CValues = std::vector<CValue>;

class CProperty;
class CStorage
{
public:
  explicit CStorage(CProperty *pProperty);
           CStorage(const CStorage &other);
           CStorage(const CStorage *other);
  virtual ~CStorage();

  void set(const std::string &value) { setValue(value); }
  void set(const int         &value) { setValue(value); }
  void set(const double      &value) { setValue(value); }
  void set(const bool        &value) { setValue(value); }
  void set(const CColor      &value) { setValue(value); }

  size_t            count() const;
  bool              empty() const;
  void              clear();
  CValues           list () const;
  const std::string type () const;
  bool            changed() const;
  bool       valueChanged();

  const CValue &last() const;
  const CValue &at(const size_t &index) const;
  void addObserver(const std::string &scriptName, const std::string &methodName, const bool &changedOnly);
  void removeObserver(const std::string &scriptName, const std::string &methodName);
  void removeObserversForScript(const std::string &scriptName);
  CProperty *property() const { return m_parentProperty; }

private:
  size_t m_historySize;
  mutable std::shared_mutex m_mutex;
  std::deque<CValue> m_history;
  std::vector<CObserver> m_observers;
  CProperty *m_parentProperty;
  CValue *m_emptyValue;

  void callObservers();

  template<typename T> void setValue(const T& value);
};

template <typename T>
inline void CStorage::setValue(const T &value)
{
  HA_LOG_DBG_INCOMING(property()->device()->name() << ":" << property()->name() << " incoming value " << value);
  std::unique_lock lock(m_mutex);
  m_history.emplace_front(value, this);
  if (m_history.size() > m_historySize) { m_history.pop_back(); }
  lock.unlock();
  callObservers();
}

}
}
