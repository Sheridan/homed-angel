#include "utils/string.h"
#include <algorithm>
#include <random>

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

std::string to_lower(const std::string &input)
{
  std::string result = input;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
      return std::tolower(c);
  });
  return result;
}

bool is_digit(const std::string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string random(const size_t &length)
{
  const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

  std::string result;
  for (size_t i = 0; i < length; ++i)
  {
    result += characters[distribution(generator)];
  }

  return result;
}
}
}
