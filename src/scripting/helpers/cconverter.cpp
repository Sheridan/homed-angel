#include "scripting/helpers/cconverter.h"
#include <regex>
#include "st.h"

namespace ha
{
namespace scripting
{
namespace helpers
{

CConverter::CConverter()
{
  m_dictonary =
  {
    { "ha::(\\w+?::)*"      , ""               },
    { "std::string"         , "string"         },
    { "\\*"                 , "@"              },
    { "unsigned\\s+char"    , "uint8"          },
    { "unsigned\\s+short"   , "uint16"         },
    { "unsigned\\s+int"     , "uint"           },
    { "unsigned\\s+uint64_t", "uint64"         },
    { "char"                , "int8"           },
    { "short"               , "int16"          },
    { "int"                 , "int"            },
    { "std::time_t"         , "int64"          },
    { "int64_t"             , "int64"          },
    { "size_t"              , "uint"           },
    { "operator\\+"         , "opAdd"          },
    { "operator-"           , "opSub"          },
    { "operator<="          , "opLessEqual"    },
    { "operator<"           , "opLess"         },
    { "operator>="          , "opGreaterEqual" },
    { "operator>"           , "opGreater"      },
    { "operator=="          , "opEquals"       },
    { "operator!="          , "opNotEquals"    },
    { "operator="           , "opAssign"       },
    { "operator\\[\\]"      , "opIndex"        }
  };
}

CConverter::~CConverter()
{}

std::string CConverter::operator()(const std::string &input)
{
  std::string result = replaceAmpersands(input);
  for (const SConverterReplacement &cr : m_dictonary)
  {
    result = std::regex_replace(result, std::regex(cr.regex), cr.replacement);
    HA_LOG_DBG_CONVERTER(cr.regex << " ->" << cr.replacement << " : " << result);
  }
  HA_LOG_DBG_CONVERTER("Converter: " << input << " -> " << result);
  return result;
}

std::string CConverter::replaceAmpersands(const std::string& input)
{
  std::regex re(R"(\(([^)]+)\))");
  std::smatch match;
  if (std::regex_search(input, match, re))
  {
    std::string params = match[1].str();
    std::string modifiedParams = std::regex_replace(params, std::regex(R"(&\s*(?!in))"), "&in ");
    return std::regex_replace(input, re, "(" + modifiedParams + ")");
  }
  return input;
}

}
}
}
