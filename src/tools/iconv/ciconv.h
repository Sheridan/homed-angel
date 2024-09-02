#pragma once
#include <string>


namespace ha
{
namespace tools
{

class CIconv
{
public:
  static std::string convert(const std::string& input, const std::string& fromEncoding, const std::string& toEncoding);
  static std::string convert(const std::string& input, const std::string& toEncoding);
  static std::string toHex(const std::string& input);
  static std::string fromHex(const std::string& input);

  static std::string toGsmUcs2(const std::string& input);
  static std::string fromGsmUcs2(const std::string& input);
};

}
}
