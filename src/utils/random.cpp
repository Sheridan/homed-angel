#include "utils/random.h"
#include <algorithm>
#include <sstream>
#include "log.h"
namespace ha
{
namespace utils
{

#define HA_MAX_RANDOM_STRING_LENGTH 65535
std::string random_string(const std::string &characters, const size_t length)
{
  size_t result_length = length > HA_MAX_RANDOM_STRING_LENGTH ? HA_MAX_RANDOM_STRING_LENGTH : length;
  // HA_LOG_DBG("rnd str length: " << length << " (" << result_length << ")");
  std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);
  std::string result;
  for (size_t i = 0; i < result_length; ++i)
  {
    result += characters[distribution(generator)];
  }
  return result;
}

std::string random_string(const size_t length)
{
  return random_string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", length);
}

std::vector<std::string> shuffle_strings(std::vector<std::string> input)
{
  std::shuffle(input.begin(), input.end(), generator);
  return input;
}

HA_RANDOM_NUMBER_REALISATION(float         ,uniform_real_distribution);
HA_RANDOM_NUMBER_REALISATION(double        ,uniform_real_distribution);
HA_RANDOM_NUMBER_REALISATION(int           ,uniform_int_distribution );
HA_RANDOM_NUMBER_REALISATION(unsigned int  ,uniform_int_distribution );
HA_RANDOM_NUMBER_REALISATION(short         ,uniform_int_distribution );
HA_RANDOM_NUMBER_REALISATION(unsigned short,uniform_int_distribution );
HA_RANDOM_NUMBER_REALISATION(int64_t       ,uniform_int_distribution );
HA_RANDOM_NUMBER_REALISATION(uint64_t      ,uniform_int_distribution );

}
}
