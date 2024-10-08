#pragma once
#include "tools/singletone/csingletone.h"

#define HA_ST ha::CSingleTone::instance()

#include "log.h"

#define HA_LOG_VERBOSE(_what) if(HA_ST->cmdLine()->option<bool>("verbose")) { HA_LOG_COUT(verbose,_what) }
