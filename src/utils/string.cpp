#include "utils/string.h"
#include <algorithm>

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

bool is_digit(const std::string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit);
}
}
}
