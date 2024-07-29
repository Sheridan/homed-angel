#pragma once
#include <string>
#include <vector>

namespace ha
{
namespace scripting
{
namespace helpers
{

struct SConverterReplacement
{
  std::string regex;
  std::string replacement;
};

class CConverter
{

public:
  CConverter();
  ~CConverter();
  std::string operator()(const std::string &input);

private:
  std::vector<SConverterReplacement> m_dictonary;
  std::string replaceAmpersands(const std::string& input);
};



}
}
}
