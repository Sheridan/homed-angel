#pragma once
#include "scripting/script/callers/ccaller.h"
#include "log.h"

namespace ha
{
namespace scripting
{

template<typename TFirst, typename TSecond>
class CCallerDouble : public CCaller
{
  struct SCallerData
  {
    std::string method;
    TFirst first;
    TSecond second;
  };

public:
  explicit CCallerDouble(CScript *parent, const std::string &params) : CCaller(parent), m_params(params) {}
  virtual  ~CCallerDouble() {};

  void append(const std::string &method, TFirst first, TSecond second)
  {
    m_queue.push({method, first, second});
  };

  void call() override
  {
    if(!m_queue.empty())
    {
      SCallerData cd = m_queue.front();
      m_queue.pop();
      script()->callMethod(cd.method, m_params, cd.first, cd.second);
    }
  };

private:
  std::queue<SCallerData> m_queue;
  std::string m_params;
};

}
}
