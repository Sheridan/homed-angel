#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "log.h"
namespace ha
{
namespace utils
{

using CStrings = std::vector<std::string>;

static const std::string empty_string = "";

std::string to_snake_case(const std::string& input);
std::string to_camel_case(const std::string& input);
std::string to_lower(const std::string& input);
std::string to_upper(const std::string& input);
bool is_digit(const std::string &str);
std::size_t calculate_hash(const std::string& input);
std::vector<std::string> sort_strings(std::vector<std::string> input);
std::vector<std::string> unique_strings(std::vector<std::string> input);

template<typename T>
std::string to_string(T value)
{
  if constexpr (std::is_same<T, bool>::value) { return value ? "true" : "false"; }
  if constexpr (std::is_arithmetic<T>::value) { return std::to_string(value);    }
  HA_LOG_ERR("to_string() error: value must be an numeric type or bool");
  return "NaN";
}

}
}
