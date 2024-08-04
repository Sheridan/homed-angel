#pragma once
#include "scripting/script/callers/ccaller.h"
#include "log.h"

namespace ha
{
namespace scripting
{

template<typename T>
class CCallerSingle : public CCaller
{
  struct SCallerData
  {
    std::string method;
    T data;
  };

public:
  explicit CCallerSingle(CScript *parent, const std::string &params) : CCaller(parent), m_params(params) {}
  virtual  ~CCallerSingle() {};

  void append(const std::string &method, T param)
  {
    m_queue.push({method, param});
  };

  void call() override
  {
    if(!m_queue.empty())
    {
      SCallerData cd = m_queue.front();
      m_queue.pop();
      script()->callMethod(cd.method, m_params, cd.data);
    }
  };

private:
  std::queue<SCallerData> m_queue;
  std::string m_params;
};

}
}
