#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "utils/string.h"
#include "log.h"

namespace ha
{
namespace homed
{

template<typename T>
using TContainer = std::vector<T>;

template<typename T>
class CContainer
{
public:
           CContainer() {}
  virtual ~CContainer()
  {
    for(T *item: m_container) { delete item; }
    m_container.clear();
  }

  virtual T *newElement(const std::string &name) = 0;

  ha::utils::CStrings list()
  {
    ha::utils::CStrings result;
    for(T *item: m_container) { if(!item->name().empty()) { result.push_back(item->name()); } }
    return result;
  }

  const bool empty() const
  {
    return m_container.empty();
  }

  bool exists(const std::string &name)
  {
     return std::any_of(m_container.begin(), m_container.end(),
                       [&name](const T *item) { return item->name() == name; });
  }

  T *add(const std::string &name)
  {
    T *t = get(name);
    if(t) { return t; }
    t = newElement(name);
    m_container.push_back(t);
    return t;
  }

  T *get(const std::string &name)
  {
    for(T *item: m_container) { if(item->name() == name) { return item; } }
    return nullptr;
  }

  T *get(const unsigned short &index)
  {
    if(index < size()) { return m_container[index]; }
    return nullptr;
  }


  T *ensure(const std::string &name)
  {
    T *t = get(name);
    if(t) { return t; }
    return add(name);
  }

  void remove(const std::string &name)
  {
    for (auto it = m_container.begin(); it != m_container.end(); it++)
    {
      if ((*it)->name() == name)
      {
        delete *it;
        m_container.erase(it);
        return;
      }
    }
  }

  unsigned short size()
  {
    return m_container.size();
  }

  TContainer<T *> &items()
  {
    return m_container;
  }
  // T* operator[](const std::string &name) { return get(name); }


private:
  TContainer<T *> m_container;
};


}
}
