#include "homed/value/cstorage.h"
#include "homed/model/entities/cproperty.h"
#include  <stdexcept>
#include "st.h"


namespace ha
{
namespace homed
{

CStorage::CStorage(CProperty *pProperty)
  : m_historySize(HA_ST->config()->historyCount()),
    m_parentProperty(pProperty),
    m_emptyValue(new CValue(this))
{}

CStorage::~CStorage()
{
  m_observers.clear();
  delete m_emptyValue;
}

bool CStorage::empty() const
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  return m_history.empty();
}

size_t CStorage::count() const
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  return m_history.size();
}

void CStorage::clear()
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  m_history.clear();
}

CValues CStorage::list() const
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  CValues result;
  result.reserve(m_history.size());
  for (const auto &item : m_history)
  {
    result.emplace_back(item);
  }
  return result;
}

const std::string CStorage::type() const
{
  if (empty())           { return "undefined"; }
  if (last().isString()) { return "string"   ; }
  if (last().isInt   ()) { return "int"      ; }
  if (last().isDouble()) { return "double"   ; }
  if (last().isBool  ()) { return "bool"     ; }
  if (last().isColor ()) { return "CColor"   ; }
  return                          "unknown"  ;
}

bool CStorage::changed() const
{
  if(empty())      { return false; }
  if(count() == 1) { return true ; }
  return last() != at(1);
}

bool CStorage::valueChanged()
{
  // HA_MUTEX_STATUS(m_mutex);
  if(empty())      { return false; }
  if(count() == 1) { return true ; }
  if(changed())
  {
    return last().value() != at(1).value();
  }
  return false;
}

const CValue &CStorage::last() const
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  if(empty())
  {
    HA_LOG_WRN(property()->device()->name() << ":" << property()->name() << " last() is empty!");
    return *m_emptyValue;
  }
  return m_history.front();
}

const CValue &CStorage::at(const size_t &index) const
{
  // HA_MUTEX_STATUS(m_mutex);
  std::shared_lock lock(m_mutex);
  if(empty()) { HA_LOG_ERR("at(" << index << ") is empty!"); }
  return m_history[index];
}

void CStorage::addObserver(const std::string &scriptName, const std::string &methodName, const bool &changedOnly)
{
  m_observers.push_back(CObserver(scriptName, methodName, changedOnly, m_parentProperty));
}

void CStorage::removeObserversForScript(const std::string &scriptName)
{
  m_observers.erase(std::remove_if(
                                    m_observers.begin(),
                                    m_observers.end(),
                                    [&scriptName](const CObserver& observer) { return observer.scriptName() == scriptName; }
                                  ),
                    m_observers.end());
}

void CStorage::callObservers()
{
  for (CObserver &observer : m_observers)
  {
    observer.call();
  }
}

}
}
