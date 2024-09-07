#include "utils/fs.h"

namespace ha
{
namespace utils
{

bool fileExists(const std::string& filename)
{
  return std::filesystem::exists(filename);
}

}
}
