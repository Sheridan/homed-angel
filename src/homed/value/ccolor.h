#pragma once
#include <array>
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

namespace ha
{
namespace homed
{

class CColor {
public:
    CColor();
    CColor(unsigned char r, unsigned char g, unsigned char b);
    CColor(const int& color);
    CColor(const std::string& color);
    CColor(const CColor& other);
    explicit CColor(const Json::Value &item);

    CColor& operator= (const CColor& other);
    bool    operator==(const CColor& other) const;
    bool    operator!=(const CColor& other) const;

    unsigned char R() const { return components[0]; }
    unsigned char G() const { return components[1]; }
    unsigned char B() const { return components[2]; }

    std::vector<int> asArray() const;
    std::string asHexString() const;

    int toInt() const;
    void fromInt(int color);
    void fromHexString(const std::string& hexString);
    const Json::Value &toJson() const;
    friend std::ostream& operator<<(std::ostream& os, const CColor& color);

private:
    std::array<unsigned char, 3> components;


// public:
//     static const CColor AliceBlue;
//     static const CColor AntiqueWhite;
//     static const CColor Aqua;
//     static const CColor Aquamarine;
//     static const CColor Azure;
//     static const CColor Beige;
//     static const CColor Bisque;
//     static const CColor Black;
//     static const CColor BlanchedAlmond;
//     static const CColor Blue;
//     static const CColor BlueViolet;
//     static const CColor Brown;
//     static const CColor BurlyWood;
//     static const CColor CadetBlue;
//     static const CColor Chartreuse;
//     static const CColor Chocolate;
//     static const CColor Coral;
//     static const CColor CornflowerBlue;
//     static const CColor Cornsilk;
//     static const CColor Crimson;
//     static const CColor Cyan;
//     static const CColor DarkBlue;
//     static const CColor DarkCyan;
//     static const CColor DarkGoldenRod;
//     static const CColor DarkGray;
//     static const CColor DarkGreen;
//     static const CColor DarkKhaki;
//     static const CColor DarkMagenta;
//     static const CColor DarkOliveGreen;
//     static const CColor DarkOrange;
//     static const CColor DarkOrchid;
//     static const CColor DarkRed;
//     static const CColor DarkSalmon;
//     static const CColor DarkSeaGreen;
//     static const CColor DarkSlateBlue;
//     static const CColor DarkSlateGray;
//     static const CColor DarkTurquoise;
//     static const CColor DarkViolet;
//     static const CColor DeepPink;
//     static const CColor DeepSkyBlue;
//     static const CColor DimGray;
//     static const CColor DodgerBlue;
//     static const CColor FireBrick;
//     static const CColor FloralWhite;
//     static const CColor ForestGreen;
//     static const CColor Fuchsia;
//     static const CColor Gainsboro;
//     static const CColor GhostWhite;
//     static const CColor Gold;
//     static const CColor GoldenRod;
//     static const CColor Gray;
//     static const CColor Green;
//     static const CColor GreenYellow;
//     static const CColor HoneyDew;
//     static const CColor HotPink;
//     static const CColor IndianRed;
//     static const CColor Indigo;
//     static const CColor Ivory;
//     static const CColor Khaki;
//     static const CColor Lavender;
//     static const CColor LavenderBlush;
//     static const CColor LawnGreen;
//     static const CColor LemonChiffon;
//     static const CColor LightBlue;
//     static const CColor LightCoral;
//     static const CColor LightCyan;
//     static const CColor LightGoldenRodYellow;
//     static const CColor LightGray;
//     static const CColor LightGreen;
//     static const CColor LightPink;
//     static const CColor LightSalmon;
//     static const CColor LightSeaGreen;
//     static const CColor LightSkyBlue;
//     static const CColor LightSlateGray;
//     static const CColor LightSteelBlue;
//     static const CColor LightYellow;
//     static const CColor Lime;
//     static const CColor LimeGreen;
//     static const CColor Linen;
//     static const CColor Magenta;
//     static const CColor Maroon;
//     static const CColor MediumAquaMarine;
//     static const CColor MediumBlue;
//     static const CColor MediumOrchid;
//     static const CColor MediumPurple;
//     static const CColor MediumSeaGreen;
//     static const CColor MediumSlateBlue;
//     static const CColor MediumSpringGreen;
//     static const CColor MediumTurquoise;
//     static const CColor MediumVioletRed;
//     static const CColor MidnightBlue;
//     static const CColor MintCream;
//     static const CColor MistyRose;
//     static const CColor Moccasin;
//     static const CColor NavajoWhite;
//     static const CColor Navy;
//     static const CColor OldLace;
//     static const CColor Olive;
//     static const CColor OliveDrab;
//     static const CColor Orange;
//     static const CColor OrangeRed;
//     static const CColor Orchid;
//     static const CColor PaleGoldenRod;
//     static const CColor PaleGreen;
//     static const CColor PaleTurquoise;
//     static const CColor PaleVioletRed;
//     static const CColor PapayaWhip;
//     static const CColor PeachPuff;
//     static const CColor Peru;
//     static const CColor Pink;
//     static const CColor Plum;
//     static const CColor PowderBlue;
//     static const CColor Purple;
//     static const CColor RebeccaPurple;
//     static const CColor Red;
//     static const CColor RosyBrown;
//     static const CColor RoyalBlue;
//     static const CColor SaddleBrown;
//     static const CColor Salmon;
//     static const CColor SandyBrown;
//     static const CColor SeaGreen;
//     static const CColor SeaShell;
//     static const CColor Sienna;
//     static const CColor Silver;
//     static const CColor SkyBlue;
//     static const CColor SlateBlue;
//     static const CColor SlateGray;
//     static const CColor Snow;
//     static const CColor SpringGreen;
//     static const CColor SteelBlue;
//     static const CColor Tan;
//     static const CColor Teal;
//     static const CColor Thistle;
//     static const CColor Tomato;
//     static const CColor Turquoise;
//     static const CColor Violet;
//     static const CColor Wheat;
//     static const CColor White;
//     static const CColor WhiteSmoke;
//     static const CColor Yellow;
//     static const CColor YellowGreen;


};

}
}
