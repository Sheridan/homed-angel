#include "homed/value/ccolor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <random>
#include "utils/string.h"
#include "st.h"
namespace ha
{
namespace homed
{

CColor::CColor()  : ha::scripting::CScriptObject(), components{0, 0, 0} {}
CColor::CColor(unsigned char r, unsigned char g, unsigned char b) : ha::scripting::CScriptObject(), components{r, g, b} {}
CColor::CColor(const int &color) { fromInt(color); }
CColor::CColor(const CColor &other) : ha::scripting::CScriptObject(), components{other.components[0], other.components[1], other.components[2]} {}
CColor::CColor(const Json::Value &item)
: ha::scripting::CScriptObject(),
  components
  {
    static_cast<unsigned char>(item[0].asInt()),
    static_cast<unsigned char>(item[1].asInt()),
    static_cast<unsigned char>(item[2].asInt())
  } {}

CColor::CColor(const std::string &color) : ha::scripting::CScriptObject()
{
       if(color[0] == '#'  ) { fromHexString(color); }
  else if(color == "random") { fromRandom()        ; }
  else                       { fromName(color)     ; }
}

CColor &CColor::operator=(const CColor &other)
{
  if (this != &other)
  {
    components[0] = other.components[0];
    components[1] = other.components[1];
    components[2] = other.components[2];
  }
  return *this;
}

bool CColor::operator==(const CColor &other) const
{
  return components[0] == other.components[0] &&
         components[1] == other.components[1] &&
         components[2] == other.components[2];
}

bool CColor::operator!=(const CColor &other) const
{
  return !(*this == other);
}

std::vector<int> CColor::asArray() const
{
  return {
          static_cast<int>(components[0]),
          static_cast<int>(components[1]),
          static_cast<int>(components[2])
         };
}

std::string CColor::asHexString() const
{
  std::ostringstream oss;
  oss << '#' << std::hex     << std::setfill('0')
             << std::setw(2) << static_cast<int>(components[0])
             << std::setw(2) << static_cast<int>(components[1])
             << std::setw(2) << static_cast<int>(components[2]);
  return oss.str();
}

int CColor::asInt() const
{
  return (components[0] << 16) | (components[1] << 8) | components[2];
}

void CColor::fromInt(int color)
{
  components[0] = (color >> 16) & 0xFF;
  components[1] = (color >> 8)  & 0xFF;
  components[2] =  color        & 0xFF;
}

Json::Value CColor::toJson() const
{
  Json::Value jsonArray(Json::arrayValue);
  jsonArray.append(static_cast<int>(components[0]));
  jsonArray.append(static_cast<int>(components[1]));
  jsonArray.append(static_cast<int>(components[2]));
  return jsonArray;
}

std::ostream &operator<<(std::ostream &os, const CColor &color)
{
    os << color.asHexString();
    return os;
}

void CColor::fromHexString(const std::string &hexString)
{
    if (hexString.size() != 7)
    {
        fromInt(0);
        return;
    }

    std::istringstream hexStream(hexString.substr(1));
    int color;
    hexStream >> std::hex >> color;
    fromInt(color);
}

void CColor::fromRandom()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);

  components[0] = dis(gen);
  components[1] = dis(gen);
  components[2] = dis(gen);
}

void CColor::fromName(const std::string &name)
{
  std::string clrstr = ha::utils::to_lower(name);
       if (clrstr == "aliceblue"           ) { components = {240, 248, 255}; }
  else if (clrstr == "antiquewhite"        ) { components = {250, 235, 215}; }
  else if (clrstr == "aqua"                ) { components = {0  , 255, 255}; }
  else if (clrstr == "aquamarine"          ) { components = {127, 255, 212}; }
  else if (clrstr == "azure"               ) { components = {240, 255, 255}; }
  else if (clrstr == "beige"               ) { components = {245, 245, 220}; }
  else if (clrstr == "bisque"              ) { components = {255, 228, 196}; }
  else if (clrstr == "black"               ) { components = {0  , 0  , 0  }; }
  else if (clrstr == "blanchedalmond"      ) { components = {255, 235, 205}; }
  else if (clrstr == "blue"                ) { components = {0  , 0  , 255}; }
  else if (clrstr == "blueviolet"          ) { components = {138, 43 , 226}; }
  else if (clrstr == "brown"               ) { components = {165, 42 , 42 }; }
  else if (clrstr == "burlywood"           ) { components = {222, 184, 135}; }
  else if (clrstr == "cadetblue"           ) { components = {95 , 158, 160}; }
  else if (clrstr == "chartreuse"          ) { components = {127, 255, 0  }; }
  else if (clrstr == "chocolate"           ) { components = {210, 105, 30 }; }
  else if (clrstr == "coral"               ) { components = {255, 127, 80 }; }
  else if (clrstr == "cornflowerblue"      ) { components = {100, 149, 237}; }
  else if (clrstr == "cornsilk"            ) { components = {255, 248, 220}; }
  else if (clrstr == "crimson"             ) { components = {220, 20 , 60 }; }
  else if (clrstr == "cyan"                ) { components = {0  , 255, 255}; }
  else if (clrstr == "darkblue"            ) { components = {0  , 0  , 139}; }
  else if (clrstr == "darkcyan"            ) { components = {0  , 139, 139}; }
  else if (clrstr == "darkgoldenrod"       ) { components = {184, 134, 11 }; }
  else if (clrstr == "darkgray"            ) { components = {169, 169, 169}; }
  else if (clrstr == "darkgreen"           ) { components = {0  , 100, 0  }; }
  else if (clrstr == "darkkhaki"           ) { components = {189, 183, 107}; }
  else if (clrstr == "darkmagenta"         ) { components = {139, 0  , 139}; }
  else if (clrstr == "darkolivegreen"      ) { components = {85 , 107, 47 }; }
  else if (clrstr == "darkorange"          ) { components = {255, 140, 0  }; }
  else if (clrstr == "darkorchid"          ) { components = {153, 50 , 204}; }
  else if (clrstr == "darkred"             ) { components = {139, 0  , 0  }; }
  else if (clrstr == "darksalmon"          ) { components = {233, 150, 122}; }
  else if (clrstr == "darkseagreen"        ) { components = {143, 188, 143}; }
  else if (clrstr == "darkslateblue"       ) { components = {72 , 61 , 139}; }
  else if (clrstr == "darkslategray"       ) { components = {47 , 79 , 79 }; }
  else if (clrstr == "darkturquoise"       ) { components = {0  , 206, 209}; }
  else if (clrstr == "darkviolet"          ) { components = {148, 0  , 211}; }
  else if (clrstr == "deeppink"            ) { components = {255, 20 , 147}; }
  else if (clrstr == "deepskyblue"         ) { components = {0  , 191, 255}; }
  else if (clrstr == "dimgray"             ) { components = {105, 105, 105}; }
  else if (clrstr == "dodgerblue"          ) { components = {30 , 144, 255}; }
  else if (clrstr == "firebrick"           ) { components = {178, 34 , 34 }; }
  else if (clrstr == "floralwhite"         ) { components = {255, 250, 240}; }
  else if (clrstr == "forestgreen"         ) { components = {34 , 139, 34 }; }
  else if (clrstr == "fuchsia"             ) { components = {255, 0  , 255}; }
  else if (clrstr == "gainsboro"           ) { components = {220, 220, 220}; }
  else if (clrstr == "ghostwhite"          ) { components = {248, 248, 255}; }
  else if (clrstr == "gold"                ) { components = {255, 215, 0  }; }
  else if (clrstr == "goldenrod"           ) { components = {218, 165, 32 }; }
  else if (clrstr == "gray"                ) { components = {128, 128, 128}; }
  else if (clrstr == "green"               ) { components = {0  , 128, 0  }; }
  else if (clrstr == "greenyellow"         ) { components = {173, 255, 47 }; }
  else if (clrstr == "honeydew"            ) { components = {240, 255, 240}; }
  else if (clrstr == "hotpink"             ) { components = {255, 105, 180}; }
  else if (clrstr == "indianred"           ) { components = {205, 92 , 92 }; }
  else if (clrstr == "indigo"              ) { components = {75 , 0  , 130}; }
  else if (clrstr == "ivory"               ) { components = {255, 255, 240}; }
  else if (clrstr == "khaki"               ) { components = {240, 230, 140}; }
  else if (clrstr == "lavender"            ) { components = {230, 230, 250}; }
  else if (clrstr == "lavenderblush"       ) { components = {255, 240, 245}; }
  else if (clrstr == "lawngreen"           ) { components = {124, 252, 0  }; }
  else if (clrstr == "lemonchiffon"        ) { components = {255, 250, 205}; }
  else if (clrstr == "lightblue"           ) { components = {173, 216, 230}; }
  else if (clrstr == "lightcoral"          ) { components = {240, 128, 128}; }
  else if (clrstr == "lightcyan"           ) { components = {224, 255, 255}; }
  else if (clrstr == "lightgoldenrodyellow") { components = {250, 250, 210}; }
  else if (clrstr == "lightgray"           ) { components = {211, 211, 211}; }
  else if (clrstr == "lightgreen"          ) { components = {144, 238, 144}; }
  else if (clrstr == "lightpink"           ) { components = {255, 182, 193}; }
  else if (clrstr == "lightsalmon"         ) { components = {255, 160, 122}; }
  else if (clrstr == "lightseagreen"       ) { components = {32 , 178, 170}; }
  else if (clrstr == "lightskyblue"        ) { components = {135, 206, 250}; }
  else if (clrstr == "lightslategray"      ) { components = {119, 136, 153}; }
  else if (clrstr == "lightsteelblue"      ) { components = {176, 196, 222}; }
  else if (clrstr == "lightyellow"         ) { components = {255, 255, 224}; }
  else if (clrstr == "lime"                ) { components = {0  , 255, 0  }; }
  else if (clrstr == "limegreen"           ) { components = {50 , 205, 50 }; }
  else if (clrstr == "linen"               ) { components = {250, 240, 230}; }
  else if (clrstr == "magenta"             ) { components = {255, 0  , 255}; }
  else if (clrstr == "maroon"              ) { components = {128, 0  , 0  }; }
  else if (clrstr == "mediumaquamarine"    ) { components = {102, 205, 170}; }
  else if (clrstr == "mediumblue"          ) { components = {0  , 0  , 205}; }
  else if (clrstr == "mediumorchid"        ) { components = {186, 85 , 211}; }
  else if (clrstr == "mediumpurple"        ) { components = {147, 112, 219}; }
  else if (clrstr == "mediumseagreen"      ) { components = {60 , 179, 113}; }
  else if (clrstr == "mediumslateblue"     ) { components = {123, 104, 238}; }
  else if (clrstr == "mediumspringgreen"   ) { components = {0  , 250, 154}; }
  else if (clrstr == "mediumturquoise"     ) { components = {72 , 209, 204}; }
  else if (clrstr == "mediumvioletred"     ) { components = {199, 21 , 133}; }
  else if (clrstr == "midnightblue"        ) { components = {25 , 25 , 112}; }
  else if (clrstr == "mintcream"           ) { components = {245, 255, 250}; }
  else if (clrstr == "mistyrose"           ) { components = {255, 228, 225}; }
  else if (clrstr == "moccasin"            ) { components = {255, 228, 181}; }
  else if (clrstr == "navajowhite"         ) { components = {255, 222, 173}; }
  else if (clrstr == "navy"                ) { components = {0  , 0  , 128}; }
  else if (clrstr == "oldlace"             ) { components = {253, 245, 230}; }
  else if (clrstr == "olive"               ) { components = {128, 128, 0  }; }
  else if (clrstr == "olivedrab"           ) { components = {107, 142, 35 }; }
  else if (clrstr == "orange"              ) { components = {255, 165, 0  }; }
  else if (clrstr == "orangered"           ) { components = {255, 69 , 0  }; }
  else if (clrstr == "orchid"              ) { components = {218, 112, 214}; }
  else if (clrstr == "palegoldenrod"       ) { components = {238, 232, 170}; }
  else if (clrstr == "palegreen"           ) { components = {152, 251, 152}; }
  else if (clrstr == "paleturquoise"       ) { components = {175, 238, 238}; }
  else if (clrstr == "palevioletred"       ) { components = {219, 112, 147}; }
  else if (clrstr == "papayawhip"          ) { components = {255, 239, 213}; }
  else if (clrstr == "peachpuff"           ) { components = {255, 218, 185}; }
  else if (clrstr == "peru"                ) { components = {205, 133, 63 }; }
  else if (clrstr == "pink"                ) { components = {255, 192, 203}; }
  else if (clrstr == "plum"                ) { components = {221, 160, 221}; }
  else if (clrstr == "powderblue"          ) { components = {176, 224, 230}; }
  else if (clrstr == "purple"              ) { components = {128, 0  , 128}; }
  else if (clrstr == "rebeccapurple"       ) { components = {102, 51 , 153}; }
  else if (clrstr == "red"                 ) { components = {255, 0  , 0  }; }
  else if (clrstr == "rosybrown"           ) { components = {188, 143, 143}; }
  else if (clrstr == "royalblue"           ) { components = {65 , 105, 225}; }
  else if (clrstr == "saddlebrown"         ) { components = {139, 69 , 19 }; }
  else if (clrstr == "salmon"              ) { components = {250, 128, 114}; }
  else if (clrstr == "sandybrown"          ) { components = {244, 164, 96 }; }
  else if (clrstr == "seagreen"            ) { components = {46 , 139, 87 }; }
  else if (clrstr == "seashell"            ) { components = {255, 245, 238}; }
  else if (clrstr == "sienna"              ) { components = {160, 82 , 45 }; }
  else if (clrstr == "silver"              ) { components = {192, 192, 192}; }
  else if (clrstr == "skyblue"             ) { components = {135, 206, 235}; }
  else if (clrstr == "slateblue"           ) { components = {106, 90 , 205}; }
  else if (clrstr == "slategray"           ) { components = {112, 128, 144}; }
  else if (clrstr == "snow"                ) { components = {255, 250, 250}; }
  else if (clrstr == "springgreen"         ) { components = {0  , 255, 127}; }
  else if (clrstr == "steelblue"           ) { components = {70 , 130, 180}; }
  else if (clrstr == "tan"                 ) { components = {210, 180, 140}; }
  else if (clrstr == "teal"                ) { components = {0  , 128, 128}; }
  else if (clrstr == "thistle"             ) { components = {216, 191, 216}; }
  else if (clrstr == "tomato"              ) { components = {255, 99 , 71 }; }
  else if (clrstr == "turquoise"           ) { components = {64 , 224, 208}; }
  else if (clrstr == "violet"              ) { components = {238, 130, 238}; }
  else if (clrstr == "wheat"               ) { components = {245, 222, 179}; }
  else if (clrstr == "white"               ) { components = {255, 255, 255}; }
  else if (clrstr == "whitesmoke"          ) { components = {245, 245, 245}; }
  else if (clrstr == "yellow"              ) { components = {255, 255, 0  }; }
  else if (clrstr == "yellowgreen"         ) { components = {154, 205, 50 }; }
  else
  {
    HA_LOG_WRN("There is no such color '" << name <<"', assigned black");
    components = {0  , 0  , 0  };
  }
}


}
}
