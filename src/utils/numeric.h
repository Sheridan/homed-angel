#pragma once
#include <string>

namespace ha
{
namespace utils
{

int         to_int    (const std::string &str);
int         hex_to_int(const std::string &str);
double      to_double (const std::string &str);
std::string int_to_hex(int value);

}
}
