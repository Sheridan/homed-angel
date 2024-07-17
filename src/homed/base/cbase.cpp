#include "homed/base/cbase.h"
#include "cbase.h"

namespace ha
{
namespace homed
{

CBase::CBase(const std::string &name)
  : m_name(name)
{}

CBase::~CBase()
{}

const std::string &CBase::name() const
{
  return m_name;
}

}
}
