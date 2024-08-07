#include "utils/thread.h"
#include <pthread.h>
#include <thread>
#include <chrono>
#include <cstring>
#include "log.h"
namespace ha
{
namespace utils
{

void set_thread_name(const std::string &name)
{
  pthread_t thread = pthread_self();
  int result = pthread_setname_np(thread, name.substr(0, 15).c_str());
  if (result != 0)
  {
    HA_LOG_ERR("Error when set thread name: " << std::strerror(result));
  }
}

void sleep(const unsigned int &milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

}
}
