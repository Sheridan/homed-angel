#include "utils/thread.h"
#include <pthread.h>
namespace ha
{
namespace utils
{

void setThreadName(const std::string &name)
{
  pthread_t thread = pthread_self();
  pthread_setname_np(thread, name.c_str());
}

}
}
