#include "utils/string.h"
#include <sstream>
#include <algorithm>
#include <functional>
#include <regex>
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
  std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c)
                  {
                    return std::tolower(c);
                  }
                );
  return result;
}

std::string to_upper(const std::string& input)
{
  std::string result = input;
  std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c)
                  {
                    return std::toupper(c);
                  }
                );
  return result;
}

bool is_digit(const std::string &str)
{
  return std::all_of(str.begin(), str.end(), ::isdigit);
}

std::size_t calculate_hash(const std::string& input)
{
  return std::hash<std::string>{}(input);
}

bool natural_compare(const std::string& a, const std::string& b)
{
  auto it_a = a.begin();
  auto it_b = b.begin();

  while (it_a != a.end() && it_b != b.end())
  {
    if (std::isdigit(*it_a) && std::isdigit(*it_b))
    {
      std::string num_a, num_b;
      while (it_a != a.end() && std::isdigit(*it_a))
      {
        num_a += *it_a++;
      }
      while (it_b != b.end() && std::isdigit(*it_b))
      {
        num_b += *it_b++;
      }
      if (num_a.size() != num_b.size())
      {
        return num_a.size() < num_b.size();
      }
      return num_a < num_b;
    }
    if (*it_a != *it_b)
    {
      return *it_a < *it_b;
    }
    ++it_a;
    ++it_b;
  }
  return it_a == a.end() && it_b != b.end();
}


std::vector<std::string> sort_strings(std::vector<std::string> input)
{
  std::sort(input.begin(), input.end(), natural_compare);
  return input;
}

std::vector<std::string> unique_strings(std::vector<std::string> input)
{
  std::sort(input.begin(), input.end());
  auto last = std::unique(input.begin(), input.end());
  input.erase(last, input.end());
  return input;
}

std::string trim(const std::string &input)
{
  auto start = input.begin();
  while (start != input.end() && std::isspace(*start))
  {
    ++start;
  }

  if (start == input.end())
  {
    return "";
  }

  auto end = input.end();
  do
  {
    --end;
  } while (end != start && std::isspace(*end));

  return std::string(start, end + 1);
}

bool contains(const std::string &input, const std::string &needle)
{
  return input.find(needle) != std::string::npos;
}

bool match(const std::string &input, const std::string &regex)
{
  return std::regex_match(input, std::regex(regex));
}

std::string extract(const std::string &input, const std::string &regex, int index)
{
  // HA_LOG_NFO("extract `" << regex << "` from `" << input << "`");
  std::smatch matches;

  if (std::regex_search(input, matches, std::regex(regex)))
  {
    return matches[index].str();
  }

  return "";
}

std::vector<std::string> split(const std::string &input, const char &delimiter)
{
  std::vector<std::string> result;
  std::string token;
  std::istringstream tokenStream(input);

  while (std::getline(tokenStream, token, delimiter))
  {
    result.push_back(token);
  }

  return result;
}

std::string replace(const std::string &input, const std::string &needle, const std::string &replacement)
{
  std::string result = input;
  size_t pos = 0;

  while ((pos = result.find(needle, pos)) != std::string::npos)
  {
    result.replace(pos, needle.length(), replacement);
    pos += replacement.length();
  }

  return result;
}

}
}
