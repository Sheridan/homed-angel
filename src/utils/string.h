#pragma once
#include <string>
#include <vector>
namespace ha
{
namespace utils
{

using CStrings = std::vector<std::string>;

static const std::string empty_string = "";
std::string to_snake_case(const std::string& input);
std::string to_lower(const std::string& input);
bool is_digit(const std::string &str);

}
}
