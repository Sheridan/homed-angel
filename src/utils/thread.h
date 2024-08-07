#pragma once
#include <string>

namespace ha
{
namespace utils
{

#define HA_DEFAULT_SLEEP_MS 100

void set_thread_name(const std::string &name);
void sleep(const unsigned int &milliseconds = HA_DEFAULT_SLEEP_MS);

}
}
