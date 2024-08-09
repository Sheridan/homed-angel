#pragma once
#include <random>
#include <string>
#include <vector>

namespace ha
{
namespace utils
{
static std::mt19937 generator(std::random_device{}());

std::string random_string(const std::string &characters, const size_t length);
std::string random_string(const size_t length);
std::vector<std::string> shuffle_strings(std::vector<std::string> input);

#define HA_RANDOM_NUMBER_DEFINITION(_type,_dist_type)  _type random(_type min, _type max)
#define HA_RANDOM_NUMBER_REALISATION(_type,_dist_type) HA_RANDOM_NUMBER_DEFINITION(_type,_dist_type) { return std::_dist_type<_type>(min, max)(generator); }

HA_RANDOM_NUMBER_DEFINITION(float         ,uniform_real_distribution);
HA_RANDOM_NUMBER_DEFINITION(double        ,uniform_real_distribution);
HA_RANDOM_NUMBER_DEFINITION(int           ,uniform_int_distribution );
HA_RANDOM_NUMBER_DEFINITION(unsigned int  ,uniform_int_distribution );
HA_RANDOM_NUMBER_DEFINITION(short         ,uniform_int_distribution );
HA_RANDOM_NUMBER_DEFINITION(unsigned short,uniform_int_distribution );
HA_RANDOM_NUMBER_DEFINITION(int64_t       ,uniform_int_distribution );
HA_RANDOM_NUMBER_DEFINITION(uint64_t      ,uniform_int_distribution );

}
}
