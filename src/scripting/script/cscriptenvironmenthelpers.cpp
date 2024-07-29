#include "homed/value/ccolor.h"
#include "datetime/timers/ctimercontinuos.h"
#include "datetime/timers/ctimeroneshoot.h"
#include "st.h"

namespace ha
{
namespace scripting
{

// CColor
ha::homed::CColor* CreateCColor()                                                  { return new ha::homed::CColor(); }
ha::homed::CColor* CreateCColor(unsigned char r, unsigned char g, unsigned char b) { return new ha::homed::CColor(r, g, b); }
ha::homed::CColor* CreateCColor(const int &color)                                  { return new ha::homed::CColor(color); }
ha::homed::CColor* CreateCColor(const std::string &color)                          { return new ha::homed::CColor(color); }
ha::homed::CColor* CreateCColor(const ha::homed::CColor &other)                    { return new ha::homed::CColor(other); }

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

// enums
#define HA_AS_ENUM(_enumname)                             HA_AS_ACCERT_CALL(m_engine->RegisterEnum(#_enumname));
#define HA_AS_ENUM_VALUE(_namespace,_enumname,_enumvalue) HA_AS_ACCERT_CALL(m_engine->RegisterEnumValue(#_enumname, #_enumvalue, _namespace::_enumname::_enumvalue));

// classes
#define HA_AS_CLASS_FACTORY_CONSTRUCTOR(_namespace,_classname,_params) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_FACTORY, m_converter(#_classname "@ f" #_params).c_str(), asFUNCTIONPR(Create##_classname, _params, _namespace::_classname *), asCALL_CDECL));

#define HA_CLASS_FACTORY_METHODS(_namespace,_classname) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_ADDREF     , "void f()"       , asMETHOD(_namespace::_classname, addRef     ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_RELEASE    , "void f()"       , asMETHOD(_namespace::_classname, release    ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_GETREFCOUNT, "int  f()"       , asMETHOD(_namespace::_classname, getRefCount), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_SETGCFLAG  , "void f()"       , asMETHOD(_namespace::_classname, setGCFlag  ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_GETGCFLAG  , "bool f()"       , asMETHOD(_namespace::_classname, getGCFlag  ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_ENUMREFS   , "void f(int &in)", asMETHOD(_namespace::_classname, enumRefs   ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_RELEASEREFS, "void f(int &in)", asMETHOD(_namespace::_classname, releaseRefs), asCALL_THISCALL));

#define HA_AS_CLASS_METHOD(_namespace,_classname,_methodname,_return,_params,_method_const) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod(#_classname, m_converter(#_return " " #_methodname #_params " " #_method_const).c_str(), asMETHODPR(_namespace::_classname, _methodname, _params _method_const, _return), asCALL_THISCALL));

#define HA_AS_CLASS_MANAGED(_namespace,_classname) HA_AS_ACCERT_CALL(m_engine->RegisterObjectType(#_classname, sizeof(_namespace::_classname), asOBJ_REF | asOBJ_NOCOUNT));
#define HA_AS_CLASS_STANDALONE(_namespace,_classname) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectType(#_classname, sizeof(_namespace::_classname), asOBJ_REF | asOBJ_GC)); \
  HA_CLASS_FACTORY_METHODS(_namespace,_classname)
}
}
