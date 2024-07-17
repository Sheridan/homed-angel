#include "homed/value/ccolor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "ccolor.h"

namespace ha
{
namespace homed
{

CColor::CColor()  : components{0, 0, 0} {}
CColor::CColor(unsigned char r, unsigned char g, unsigned char b) : components{r, g, b} {}
CColor::CColor(const int &color) { fromInt(color); }
CColor::CColor(const std::string &color) { fromHexString(color); }
CColor::CColor(const CColor &other) : components{other.components[0], other.components[1], other.components[2]} {}
CColor::CColor(const Json::Value &item)
: components
  {
    static_cast<unsigned char>(item[0].asInt()),
    static_cast<unsigned char>(item[1].asInt()),
    static_cast<unsigned char>(item[2].asInt())
  } {}

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
  oss << std::hex << std::setfill('0')
      << std::setw(2) << static_cast<int>(components[0])
      << std::setw(2) << static_cast<int>(components[1])
      << std::setw(2) << static_cast<int>(components[2]);
  return oss.str();
}

int CColor::toInt() const
{
  return (components[0] << 16) | (components[1] << 8) | components[2];
}

void CColor::fromInt(int color)
{
  components[0] = (color >> 16) & 0xFF;
  components[1] = (color >> 8)  & 0xFF;
  components[2] =  color        & 0xFF;
}

void CColor::fromHexString(const std::string &hexString)
{
  if (hexString.size() != 6) { fromInt(0); return; }

  std::istringstream hexStream(hexString);
  int color;
  hexStream >> std::hex >> color;
  fromInt(color);
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

// const CColor CColor::AliceBlue            = CColor(240, 248, 255);
// const CColor CColor::AntiqueWhite         = CColor(250, 235, 215);
// const CColor CColor::Aqua                 = CColor(0,   255, 255);
// const CColor CColor::Aquamarine           = CColor(127, 255, 212);
// const CColor CColor::Azure                = CColor(240, 255, 255);
// const CColor CColor::Beige                = CColor(245, 245, 220);
// const CColor CColor::Bisque               = CColor(255, 228, 196);
// const CColor CColor::Black                = CColor(0,   0,   0);
// const CColor CColor::BlanchedAlmond       = CColor(255, 235, 205);
// const CColor CColor::Blue                 = CColor(0,   0,   255);
// const CColor CColor::BlueViolet           = CColor(138, 43,  226);
// const CColor CColor::Brown                = CColor(165, 42,  42);
// const CColor CColor::BurlyWood            = CColor(222, 184, 135);
// const CColor CColor::CadetBlue            = CColor(95,  158, 160);
// const CColor CColor::Chartreuse           = CColor(127, 255, 0);
// const CColor CColor::Chocolate            = CColor(210, 105, 30);
// const CColor CColor::Coral                = CColor(255, 127, 80);
// const CColor CColor::CornflowerBlue       = CColor(100, 149, 237);
// const CColor CColor::Cornsilk             = CColor(255, 248, 220);
// const CColor CColor::Crimson              = CColor(220, 20,  60);
// const CColor CColor::Cyan                 = CColor(0,   255, 255);
// const CColor CColor::DarkBlue             = CColor(0,   0,   139);
// const CColor CColor::DarkCyan             = CColor(0,   139, 139);
// const CColor CColor::DarkGoldenRod        = CColor(184, 134, 11);
// const CColor CColor::DarkGray             = CColor(169, 169, 169);
// const CColor CColor::DarkGreen            = CColor(0,   100, 0);
// const CColor CColor::DarkKhaki            = CColor(189, 183, 107);
// const CColor CColor::DarkMagenta          = CColor(139, 0,   139);
// const CColor CColor::DarkOliveGreen       = CColor(85,  107, 47);
// const CColor CColor::DarkOrange           = CColor(255, 140, 0);
// const CColor CColor::DarkOrchid           = CColor(153, 50,  204);
// const CColor CColor::DarkRed              = CColor(139, 0,   0);
// const CColor CColor::DarkSalmon           = CColor(233, 150, 122);
// const CColor CColor::DarkSeaGreen         = CColor(143, 188, 143);
// const CColor CColor::DarkSlateBlue        = CColor(72,  61,  139);
// const CColor CColor::DarkSlateGray        = CColor(47,  79,  79);
// const CColor CColor::DarkTurquoise        = CColor(0,   206, 209);
// const CColor CColor::DarkViolet           = CColor(148, 0,   211);
// const CColor CColor::DeepPink             = CColor(255, 20,  147);
// const CColor CColor::DeepSkyBlue          = CColor(0,   191, 255);
// const CColor CColor::DimGray              = CColor(105, 105, 105);
// const CColor CColor::DodgerBlue           = CColor(30,  144, 255);
// const CColor CColor::FireBrick            = CColor(178, 34,  34);
// const CColor CColor::FloralWhite          = CColor(255, 250, 240);
// const CColor CColor::ForestGreen          = CColor(34,  139, 34);
// const CColor CColor::Fuchsia              = CColor(255, 0,   255);
// const CColor CColor::Gainsboro            = CColor(220, 220, 220);
// const CColor CColor::GhostWhite           = CColor(248, 248, 255);
// const CColor CColor::Gold                 = CColor(255, 215, 0);
// const CColor CColor::GoldenRod            = CColor(218, 165, 32);
// const CColor CColor::Gray                 = CColor(128, 128, 128);
// const CColor CColor::Green                = CColor(0,   128, 0);
// const CColor CColor::GreenYellow          = CColor(173, 255, 47);
// const CColor CColor::HoneyDew             = CColor(240, 255, 240);
// const CColor CColor::HotPink              = CColor(255, 105, 180);
// const CColor CColor::IndianRed            = CColor(205, 92,  92);
// const CColor CColor::Indigo               = CColor(75,  0,   130);
// const CColor CColor::Ivory                = CColor(255, 255, 240);
// const CColor CColor::Khaki                = CColor(240, 230, 140);
// const CColor CColor::Lavender             = CColor(230, 230, 250);
// const CColor CColor::LavenderBlush        = CColor(255, 240, 245);
// const CColor CColor::LawnGreen            = CColor(124, 252, 0);
// const CColor CColor::LemonChiffon         = CColor(255, 250, 205);
// const CColor CColor::LightBlue            = CColor(173, 216, 230);
// const CColor CColor::LightCoral           = CColor(240, 128, 128);
// const CColor CColor::LightCyan            = CColor(224, 255, 255);
// const CColor CColor::LightGoldenRodYellow = CColor(250, 250, 210);
// const CColor CColor::LightGray            = CColor(211, 211, 211);
// const CColor CColor::LightGreen           = CColor(144, 238, 144);
// const CColor CColor::LightPink            = CColor(255, 182, 193);
// const CColor CColor::LightSalmon          = CColor(255, 160, 122);
// const CColor CColor::LightSeaGreen        = CColor(32,  178, 170);
// const CColor CColor::LightSkyBlue         = CColor(135, 206, 250);
// const CColor CColor::LightSlateGray       = CColor(119, 136, 153);
// const CColor CColor::LightSteelBlue       = CColor(176, 196, 222);
// const CColor CColor::LightYellow          = CColor(255, 255, 224);
// const CColor CColor::Lime                 = CColor(0,   255, 0);
// const CColor CColor::LimeGreen            = CColor(50,  205, 50);
// const CColor CColor::Linen                = CColor(250, 240, 230);
// const CColor CColor::Magenta              = CColor(255, 0,   255);
// const CColor CColor::Maroon               = CColor(128, 0,   0);
// const CColor CColor::MediumAquaMarine     = CColor(102, 205, 170);
// const CColor CColor::MediumBlue           = CColor(0,   0,   205);
// const CColor CColor::MediumOrchid         = CColor(186, 85,  211);
// const CColor CColor::MediumPurple         = CColor(147, 112, 219);
// const CColor CColor::MediumSeaGreen       = CColor(60,  179, 113);
// const CColor CColor::MediumSlateBlue      = CColor(123, 104, 238);
// const CColor CColor::MediumSpringGreen    = CColor(0,   250, 154);
// const CColor CColor::MediumTurquoise      = CColor(72,  209, 204);
// const CColor CColor::MediumVioletRed      = CColor(199, 21,  133);
// const CColor CColor::MidnightBlue         = CColor(25,  25,  112);
// const CColor CColor::MintCream            = CColor(245, 255, 250);
// const CColor CColor::MistyRose            = CColor(255, 228, 225);
// const CColor CColor::Moccasin             = CColor(255, 228, 181);
// const CColor CColor::NavajoWhite          = CColor(255, 222, 173);
// const CColor CColor::Navy                 = CColor(0,   0,   128);
// const CColor CColor::OldLace              = CColor(253, 245, 230);
// const CColor CColor::Olive                = CColor(128, 128, 0);
// const CColor CColor::OliveDrab            = CColor(107, 142, 35);
// const CColor CColor::Orange               = CColor(255, 165, 0);
// const CColor CColor::OrangeRed            = CColor(255, 69,  0);
// const CColor CColor::Orchid               = CColor(218, 112, 214);
// const CColor CColor::PaleGoldenRod        = CColor(238, 232, 170);
// const CColor CColor::PaleGreen            = CColor(152, 251, 152);
// const CColor CColor::PaleTurquoise        = CColor(175, 238, 238);
// const CColor CColor::PaleVioletRed        = CColor(219, 112, 147);
// const CColor CColor::PapayaWhip           = CColor(255, 239, 213);
// const CColor CColor::PeachPuff            = CColor(255, 218, 185);
// const CColor CColor::Peru                 = CColor(205, 133, 63);
// const CColor CColor::Pink                 = CColor(255, 192, 203);
// const CColor CColor::Plum                 = CColor(221, 160, 221);
// const CColor CColor::PowderBlue           = CColor(176, 224, 230);
// const CColor CColor::Purple               = CColor(128, 0,   128);
// const CColor CColor::RebeccaPurple        = CColor(102, 51,  153);
// const CColor CColor::Red                  = CColor(255, 0,   0);
// const CColor CColor::RosyBrown            = CColor(188, 143, 143);
// const CColor CColor::RoyalBlue            = CColor(65,  105, 225);
// const CColor CColor::SaddleBrown          = CColor(139, 69,  19);
// const CColor CColor::Salmon               = CColor(250, 128, 114);
// const CColor CColor::SandyBrown           = CColor(244, 164, 96);
// const CColor CColor::SeaGreen             = CColor(46,  139, 87);
// const CColor CColor::SeaShell             = CColor(255, 245, 238);
// const CColor CColor::Sienna               = CColor(160, 82,  45);
// const CColor CColor::Silver               = CColor(192, 192, 192);
// const CColor CColor::SkyBlue              = CColor(135, 206, 235);
// const CColor CColor::SlateBlue            = CColor(106, 90,  205);
// const CColor CColor::SlateGray            = CColor(112, 128, 144);
// const CColor CColor::Snow                 = CColor(255, 250, 250);
// const CColor CColor::SpringGreen          = CColor(0,   255, 127);
// const CColor CColor::SteelBlue            = CColor(70,  130, 180);
// const CColor CColor::Tan                  = CColor(210, 180, 140);
// const CColor CColor::Teal                 = CColor(0,   128, 128);
// const CColor CColor::Thistle              = CColor(216, 191, 216);
// const CColor CColor::Tomato               = CColor(255, 99,  71);
// const CColor CColor::Turquoise            = CColor(64,  224, 208);
// const CColor CColor::Violet               = CColor(238, 130, 238);
// const CColor CColor::Wheat                = CColor(245, 222, 179);
// const CColor CColor::White                = CColor(255, 255, 255);
// const CColor CColor::WhiteSmoke           = CColor(245, 245, 245);
// const CColor CColor::Yellow               = CColor(255, 255, 0);
// const CColor CColor::YellowGreen          = CColor(154, 205, 50);

}
}
