#pragma once
#include <string>

namespace ha
{
namespace homed
{

class CBase
{
public:
  explicit CBase(const std::string &name);
  virtual ~CBase();
  virtual const std::string &name() const;

private:
  std::string m_name;
};

#define HA_OBJECT_PROPERTY(_type,_name,_default) \
private  : _type m_##_name = _default; \
public   : const _type& _name() const { return m_##_name; } \
                   void _name(const _type & val) { m_##_name = val; };

}
}
