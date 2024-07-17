#include "tools/singletone/csingletone.h"
#include <thread>
#include <iostream>
#include <stdlib.h>
#include "st.h"

namespace ha
{

CSingleTone &CSingleTone::instance()
{
  static ha::CSingleTone instance;
  return instance;
}

void CSingleTone::destruct()
{
  delete this;
}

void CSingleTone::sleep(const unsigned int &seconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(seconds));
}

CSingleTone::CSingleTone() {}
CSingleTone::~CSingleTone() {}

}
