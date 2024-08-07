#include "tools/singletone/csingletone.h"
#include "st.h"

namespace ha
{

CSingleTone *CSingleTone::instance()
{
  static ha::CSingleTone instance;
  return &instance;
}

void CSingleTone::destruct()
{
  delete this;
}

}
