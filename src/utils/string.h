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

static constexpr std::string empty_string("");
static constexpr std::string str_CR  (1, static_cast<unsigned char>(13)); // '\r'
static constexpr std::string str_LF  (1, static_cast<unsigned char>(10)); // '\n'
static constexpr std::string str_SUB (1, static_cast<unsigned char>(26)); // '\x1A'
static constexpr std::string str_NULL(1, static_cast<unsigned char>(0 )); // '\0'
static constexpr std::string str_ETX (1, static_cast<unsigned char>(3 )); // '\x03'
static constexpr std::string str_CRLF = str_CR + str_LF;                  // "\r\n"


std::string to_snake_case(const std::string& input);
std::string to_camel_case(const std::string& input);
std::string to_lower(const std::string& input);
std::string to_upper(const std::string& input);
bool is_digit(const std::string &str);
std::size_t calculate_hash(const std::string& input);
std::vector<std::string> sort_strings(std::vector<std::string> input);
std::vector<std::string> unique_strings(std::vector<std::string> input);
std::string trim(const std::string &input);
bool contains(const std::string &input, const std::string &needle);
bool match(const std::string &input, const std::string &regex);
std::string extract(const std::string &input, const std::string &regex, int index = 1);
std::vector<std::string> split(const std::string &input, const char &delimiter);
std::string replace(const std::string &input, const std::string &needle, const std::string &replacement);

template<typename T>
std::string to_string(T input)
{
  if constexpr (std::is_same<T, bool>::value) { return input ? "true" : "false"; }
  if constexpr (std::is_arithmetic<T>::value) { return std::to_string(input);    }
  HA_LOG_ERR("to_string() error: value must be an numeric type or bool");
  return "NaN";
}

}
}
