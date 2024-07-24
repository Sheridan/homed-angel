#include "homed/value/ccolor.h"
#include "datetime/timers/ctimercontinuos.h"
#include "datetime/timers/ctimeroneshoot.h"
#include "st.h"

namespace ha
{
namespace scripting
{
// using uint8 = unsigned char;
// using string = std::string;
// using CColor = ha::homed::CColor;

// CColor
ha::homed::CColor* CreateCColor()                                                  { return new ha::homed::CColor(); }
ha::homed::CColor* CreateCColor(unsigned char r, unsigned char g, unsigned char b) { return new ha::homed::CColor(r, g, b); }
ha::homed::CColor* CreateCColor(const int &color)                                  { return new ha::homed::CColor(color); }
ha::homed::CColor* CreateCColor(const std::string &color)                          { return new ha::homed::CColor(color); }
ha::homed::CColor* CreateCColor(const ha::homed::CColor &other)                    { return new ha::homed::CColor(other); }
// ha::homed::CColor* CColorDestructor   () { self->~CColor(); };

// CTimerContinuous
ha::datetime::CTimerContinuous* CreateCTimerContinuous(const std::string &scriptName, const std::string &functionName, const int64_t &interval) { return new ha::datetime::CTimerContinuous(scriptName, functionName, static_cast<std::chrono::milliseconds>(interval)); }

// CTimerOneshot
ha::datetime::CTimerOneshot* CreateCTimerOneshot(const std::string &scriptName, const std::string &functionName, const int64_t &interval) { return new ha::datetime::CTimerOneshot(scriptName, functionName, static_cast<std::chrono::milliseconds>(interval)); }

// CDateTime
ha::datetime::CDateTime* CreateCDateTime()                                                                                { return new ha::datetime::CDateTime(); }
ha::datetime::CDateTime* CreateCDateTime(int64_t timestamp)                                                               { return new ha::datetime::CDateTime(timestamp); }
ha::datetime::CDateTime* CreateCDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) { return new ha::datetime::CDateTime(year, month, day, hour, minute, second, millisecond); }

// CTimeInterval
ha::datetime::CTimeInterval* CreateCTimeInterval(double seconds) { return new ha::datetime::CTimeInterval(seconds); }

// HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CDateTime", asBEHAVE_FACTORY    , "CDateTime@ f()", asFUNCTIONPR(CreateCDateTime, (), ha::datetime::CDateTime*), asCALL_CDECL));
#define HA_CLASS_FACTORY_CONSTRUCTOR(_namespace,_classname,_as_params,_params) \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_FACTORY, #_classname "@ f" _as_params , asFUNCTIONPR(Create##_classname, _params, _namespace::_classname *), asCALL_CDECL));

#define HA_CLASS_FACTORY_METHODS(_type,_classname) \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_ADDREF     , "void f()"      , asMETHOD(_type, addRef     ), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_RELEASE    , "void f()"      , asMETHOD(_type, release    ), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_GETREFCOUNT, "int f()"       , asMETHOD(_type, getRefCount), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_SETGCFLAG  , "void f()"      , asMETHOD(_type, setGCFlag  ), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_GETGCFLAG  , "bool f()"      , asMETHOD(_type, getGCFlag  ), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_ENUMREFS   , "void f(int&in)", asMETHOD(_type, enumRefs   ), asCALL_THISCALL)); \
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour(_classname, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(_type, releaseRefs), asCALL_THISCALL));

}
}
