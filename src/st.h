#pragma once
// #include <stdexcept>
// #include <chrono>
#include "tools/singletone/csingletone.h"


// #define HA_STR_HELPER(x) #x
// #define HA_STR(x) HA_STR_HELPER(x)

#define HA_ST ha::CSingleTone::instance()

#include "log.h"

#define HA_LOG_VERBOSE(_what) if(HA_ST.cmdLine().option<bool>("verbose")) { HA_LOG_NFO(_what) }
