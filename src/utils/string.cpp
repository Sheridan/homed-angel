#include "utils/string.h"
#include <algorithm>
#include <random>
#include <functional>
#include "log.h"
namespace ha
{
namespace utils
{

std::string to_snake_case(const std::string& input)
{
  std::string result;
  for (char ch : input)
  {
    if (std::isupper(ch))
    {
      if (!result.empty())
      {
        result += '_';
      }
      result += std::tolower(ch);
    }
    else
    {
      result += ch;
    }
  }
  return result;
}

std::string to_camel_case(const std::string& input)
{
  std::string result;
  bool to_upper = false;
  for (char ch : input)
  {
    if (ch == '_')
    {
      to_upper = true;
    }
    else
    {
      if (to_upper)
      {
        result += std::toupper(ch);
        to_upper = false;
      }
      else
      {
        result += ch;
      }
    }
  }
  return result;
}

std::string to_lower(const std::string &input)
{
  std::string result = input;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
      return std::tolower(c);
  });
  return result;
}

std::string to_upper(const std::string& input)
{
  std::string result = input;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
      return std::toupper(c);
  });
  return result;
}

bool is_digit(const std::string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit);
}

#define HA_MAX_RANDOM_STRING_LENGTH 65535
std::string random(const size_t length)
{
  size_t result_length = length > HA_MAX_RANDOM_STRING_LENGTH ? HA_MAX_RANDOM_STRING_LENGTH : length;
  HA_LOG_DBG("rnd str length: " << length << " (" << result_length << ")");
  const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

  std::string result;
  for (size_t i = 0; i < result_length; ++i)
  {
    result += characters[distribution(generator)];
  }

  return result;
}

std::size_t calculate_hash(const std::string& input)
{
  std::hash<std::string> hasher;
  return hasher(input);
}

}
}
