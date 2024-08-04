#include "homed/value/ccolor.h"
#include "datetime/timers/ctimercontinuos.h"
#include "datetime/timers/ctimeroneshoot.h"
#include "datetime/timers/ctimercron.h"
#include "st.h"

namespace ha
{
namespace scripting
{

template<typename T> void asDestructor(void *mem)
{
    static_cast<T *>(mem)->~T();
}

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

// CTimerCron
ha::datetime::CTimerCron* CreateCTimerCron(const std::string &scriptName, const std::string &functionName, const std::string& schedule) { return new ha::datetime::CTimerCron(scriptName, functionName, schedule); }


// CDateTime
void CreateCDateTime     (void *memory)                                                                                  { new(memory) ha::datetime::CDateTime(); }
void CreateCDateTimeCopy (void *memory, const ha::datetime::CDateTime& other)                                            { new(memory) ha::datetime::CDateTime(other); }
void CreateCDateTimeTS   (void *memory, int64_t timestamp)                                                               { new(memory) ha::datetime::CDateTime(timestamp); }
void CreateCDateTimeParts(void *memory, int year, int month, int day, int hour, int minute, int second, int millisecond) { new(memory) ha::datetime::CDateTime(year, month, day, hour, minute, second, millisecond); }
// HA_AS_CLASS_VALUE_DESTRUCTOR_METHOD(ha::datetime, CDateTime);

// CTimeInterval
void CreateCTimeInterval    (void *memory, double seconds)                           { new(memory) ha::datetime::CTimeInterval(seconds); }
void CreateCTimeIntervalCopy(void *memory, const ha::datetime::CTimeInterval& other) { new(memory) ha::datetime::CTimeInterval(other); }
// HA_AS_CLASS_VALUE_DESTRUCTOR_METHOD(ha::datetime, CTimeInterval);

// enums
#define HA_AS_ENUM(_enumname)                             HA_AS_ACCERT_CALL(m_engine->RegisterEnum(#_enumname));
#define HA_AS_ENUM_VALUE(_namespace,_enumname,_enumvalue) HA_AS_ACCERT_CALL(m_engine->RegisterEnumValue(#_enumname, #_enumvalue, _namespace::_enumname::_enumvalue));

// classes
#define HA_AS_CLASS_METHOD(_namespace,_classname,_methodname,_return,_params,_method_const) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod(#_classname, m_converter(#_return " " #_methodname #_params " " #_method_const).c_str(), asMETHODPR(_namespace::_classname, _methodname, _params _method_const, _return), asCALL_THISCALL)); \
  HA_LOG_DBG_CONVERTER(#_return " " #_methodname #_params " " #_method_const << "  ->  " << m_converter(#_return " " #_methodname #_params " " #_method_const))

// managed
#define HA_AS_CLASS_MANAGED(_namespace,_classname) HA_AS_ACCERT_CALL(m_engine->RegisterObjectType(#_classname, sizeof(_namespace::_classname), asOBJ_REF | asOBJ_NOCOUNT));

// value
#define HA_AS_CLASS_VALUE(_namespace,_classname) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectType(#_classname, sizeof(_namespace::_classname), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK)); \

#define HA_AS_CLASS_VALUE_CONSTRUCTOR(_namespace,_classname,_params,_as_suffix) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_CONSTRUCT, m_converter("void f" #_params).c_str(), asFUNCTION(Create##_classname##_as_suffix), asCALL_CDECL_OBJFIRST));

#define HA_AS_CLASS_VALUE_DESTRUCTOR(_namespace,_classname) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(asDestructor<_namespace::_classname>), asCALL_CDECL_OBJFIRST));

// smart
#define HA_AS_CLASS_SMART(_namespace,_classname) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectType(#_classname, sizeof(_namespace::_classname), asOBJ_REF | asOBJ_GC)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_ADDREF     , "void f()"       , asMETHOD(_namespace::_classname, addRef     ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_RELEASE    , "void f()"       , asMETHOD(_namespace::_classname, release    ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_GETREFCOUNT, "int  f()"       , asMETHOD(_namespace::_classname, getRefCount), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_SETGCFLAG  , "void f()"       , asMETHOD(_namespace::_classname, setGCFlag  ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_GETGCFLAG  , "bool f()"       , asMETHOD(_namespace::_classname, getGCFlag  ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_ENUMREFS   , "void f(int &in)", asMETHOD(_namespace::_classname, enumRefs   ), asCALL_THISCALL)); \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_RELEASEREFS, "void f(int &in)", asMETHOD(_namespace::_classname, releaseRefs), asCALL_THISCALL));

#define HA_AS_CLASS_SMART_CONSTRUCTOR(_namespace,_classname,_params) \
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectBehaviour(#_classname, asBEHAVE_FACTORY, m_converter(#_classname "@ f" #_params).c_str(), asFUNCTIONPR(Create##_classname, _params, _namespace::_classname *), asCALL_CDECL));

}
}
