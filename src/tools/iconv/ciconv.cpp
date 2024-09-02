#include "tools/iconv/ciconv.h"
#include <iconv.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "ciconv.h"

namespace ha
{
namespace tools
{

std::string CIconv::convert(const std::string& input, const std::string& fromEncoding, const std::string& toEncoding)
{
  iconv_t cd = iconv_open(toEncoding.c_str(), fromEncoding.c_str());
  if (cd == (iconv_t)-1)
  {
    throw std::runtime_error("Can not open iconv" + std::string(strerror(errno)));
  }

  size_t inBytesLeft = input.size();
  size_t outBytesLeft = inBytesLeft * 4;
  char* inBuf = const_cast<char*>(input.c_str());
  char* outBuf = new char[outBytesLeft];
  char* outPtr = outBuf;

  if (iconv(cd, &inBuf, &inBytesLeft, &outPtr, &outBytesLeft) == (size_t)-1)
  {
    std::string errorMsg = "Conversion error from " + fromEncoding + " to " + toEncoding;
    switch (errno)
    {
      case E2BIG:
        errorMsg += "Output buffer is too small.";
        break;
      case EILSEQ:
        errorMsg += "Input contains invalid characters.";
        break;
      case EINVAL:
        errorMsg += "Incomplete multibyte sequence.";
        break;
      default:
        errorMsg += "Unknown error occurred.";
        break;
    }

    delete[] outBuf;
    iconv_close(cd);
    throw std::runtime_error(errorMsg);
  }

  std::string result(outBuf, outPtr - outBuf);
  delete[] outBuf;
  iconv_close(cd);
  return result;
}

std::string CIconv::convert(const std::string &input, const std::string &toEncoding)
{
  return convert(input, "UTF-8", toEncoding);
}

std::string CIconv::toHex(const std::string &input)
{
  std::ostringstream oss;
  oss << std::uppercase;
  for (unsigned char c : input) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
  }
  return oss.str();
}

std::string CIconv::fromHex(const std::string& input)
{
  std::string output;
  for (size_t i = 0; i < input.length(); i += 2) {
    std::string byteString = input.substr(i, 2);
    char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
    output.push_back(byte);
  }
  return output;
}

std::string CIconv::toGsmUcs2(const std::string& input)
{
  return toHex(convert(input, "UCS-2BE"));
}

std::string CIconv::fromGsmUcs2(const std::string& input)
{
  return convert(fromHex(input), "UCS-2BE", "UTF-8");
}

}
}
