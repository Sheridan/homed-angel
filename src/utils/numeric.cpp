#include "utils/numeric.h"
#include <cstdlib>
#include <cerrno>
#include <functional>
#include <sstream>
#include <iomanip>
#include "log.h"

namespace ha
{
namespace utils
{

#define HA_SRING_TO_NUMERIC(_type, _convert) \
{ \
  errno = 0; \
  char *endptr = nullptr; \
  _type result = _convert; \
  if (errno != 0 || *endptr != '\0' || endptr == str.c_str()) \
  { \
    HA_LOG_ERR("Can not convert string '" << str << "' to " #_type); \
    return 0; \
  } \
  return result; \
}

int    to_int    (const std::string &str) { HA_SRING_TO_NUMERIC(int   , std::strtol(str.c_str(), &endptr, 10)); }
int    hex_to_int(const std::string &str) { HA_SRING_TO_NUMERIC(int   , std::strtol(str.c_str(), &endptr, 16)); }
double to_double (const std::string &str) { HA_SRING_TO_NUMERIC(double, std::strtod(str.c_str(), &endptr    )); }

std::string int_to_hex(int value)
{
  std::stringstream stream;
  stream << std::hex << std::uppercase << value;
  return stream.str();
}

}
}
