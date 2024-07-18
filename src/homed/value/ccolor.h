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
    int asInt() const;

    Json::Value toJson() const;
    friend std::ostream& operator<<(std::ostream& os, const CColor& color);

private:
    std::array<unsigned char, 3> components;

    void fromInt(int color);
    void fromHexString(const std::string& hexString);
    void fromName(const std::string& name);
    void fromRandom();
};

}
}
