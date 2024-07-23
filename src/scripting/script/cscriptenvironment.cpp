#include "scripting/script/cscriptenvironment.h"
#include <fstream>
#include <string>

#include "sdk/add_on/scriptstdstring/scriptstdstring.h"
#include "sdk/add_on/scriptarray/scriptarray.h"
#include "sdk/tests/test_feature/source/stdvector.h"

#include "datetime/astronomical/csuntracker.h"
#include "homed/chomed.h"
#include "st.h"

#include "scripting/script/cscriptenvironmenthelpers.cpp"
namespace ha
{
namespace scripting
{

CScriptEnvironment::CScriptEnvironment(const std::filesystem::path &file)
  : m_builder(nullptr),
    m_context(nullptr),
    m_file(file),
    m_name(extractName(file))
    // m_module(nullptr)
{
  m_logger = new ha::scripting::helpers::CLogger(m_name);
  m_engine = asCreateScriptEngine();
  assert(m_engine != nullptr && "Failed to create AngelScript m_engine");
}

CScriptEnvironment::~CScriptEnvironment()
{
  m_context->Release();
  delete m_builder;
  delete m_logger;
  if (m_engine)
  {
    m_engine->ShutDownAndRelease();
  }
}

std::string CScriptEnvironment::content()
{
  std::ifstream ifs(m_file.string());
  return std::string ( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
}

std::string CScriptEnvironment::extractName(const std::filesystem::path& file)
{
  std::filesystem::path relative_path = std::filesystem::relative(file, HA_ST.config().scriptingLocation());
  relative_path = relative_path.parent_path() / relative_path.stem();

  std::string transformed_path;
  for (auto it = relative_path.begin(); it != relative_path.end(); ++it)
  {
    transformed_path += it->string();
    if (std::next(it) != relative_path.end())
    {
      transformed_path += ':';
    }
  }

  return transformed_path;
}

void CScriptEnvironment::messageCallback(const asSMessageInfo *msg)
{
                                     const char* type = "err";
       if (msg->type == asMSGTYPE_WARNING)     { type = "wrn"; }
  else if (msg->type == asMSGTYPE_INFORMATION) { type = "inf"; }
  HA_LOG_SCRIPT(type, msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message);
}

void CScriptEnvironment::registerEntities()
{
  HA_ACCERT_CALL(m_engine->SetMessageCallback(asMETHOD(CScriptEnvironment, messageCallback), this, asCALL_THISCALL));
  registerBase();
  registerLogger();
  registerModel();
  registerVariables();
}

bool CScriptEnvironment::build()
{
  m_builder = new CScriptBuilder();

  m_context = m_engine->CreateContext();
  if(!m_context) { HA_LOG_ERR("[" << m_name << "] Can not create context"); return false; }
  if(m_builder->StartNewModule(m_engine,    name().c_str()) < 0) { HA_LOG_ERR("[" << m_name << "] Failed to start new module"); return false; }
  if(m_builder->AddSectionFromFile(file().string().c_str()) < 0) { HA_LOG_ERR("[" << m_name << "] Failed to add script file" ); return false; }
  if(m_builder->BuildModule()                               < 0) { HA_LOG_ERR("[" << m_name << "] Failed to build the module"); return false; }
  // m_module = m_engine->GetModule(m_name.c_str(), asGM_ALWAYS_CREATE);
  // m_module->AddSectionFromFile(file().string().c_str());
  // if(m_module->Build() < 0) { HA_LOG_ERR("[" << m_name << "] Can not build"); return false; }
  return true;
}

void CScriptEnvironment::registerBase()
{
  RegisterScriptArray(m_engine, true);
  RegisterStdString(m_engine);
  RegisterStdStringUtils(m_engine);
  RegisterVector<std::string>("CStrings", "string", m_engine);
}

void CScriptEnvironment::registerLogger()
{
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CLogger", 0, asOBJ_REF | asOBJ_NOCOUNT));

// // Регистрация конструктора
// r = m_engine->RegisterObjectBehaviour("CLogger", asBEHAVE_CONSTRUCT, "void f(const string &in)", asFUNCTION(ConstructCLogger), asCALL_CDECL_OBJLAST); assert(r >= 0);

  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CLogger", "void dbg(const string &in)", asMETHOD(ha::scripting::helpers::CLogger, dbg), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CLogger", "void nfo(const string &in)", asMETHOD(ha::scripting::helpers::CLogger, nfo), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CLogger", "void wrn(const string &in)", asMETHOD(ha::scripting::helpers::CLogger, wrn), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CLogger", "void err(const string &in)", asMETHOD(ha::scripting::helpers::CLogger, err), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CLogger", "void cry(const string &in)", asMETHOD(ha::scripting::helpers::CLogger, cry), asCALL_THISCALL));
}

void CScriptEnvironment::registerModel()
{
  // types
  HA_ACCERT_CALL(m_engine->RegisterEnum("EDeviceType"));
  HA_ACCERT_CALL(m_engine->RegisterEnum("EPropertyValueType"));
  HA_ACCERT_CALL(m_engine->RegisterEnum("ESunTrackerEvent"));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CColor"          , sizeof(ha::homed::CColor), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CValue"          , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CStorage"        , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CProperty"       , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CProperties"     , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CEndpoint"       , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CEndpoints"      , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CDevice"         , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CDevices"        , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CHomed"          , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CSunTracker"     , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CAstronomical"   , 0, asOBJ_REF | asOBJ_NOCOUNT));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CTimerContinuous", sizeof(ha::datetime::CTimerContinuous), asOBJ_REF | asOBJ_GC));
  HA_ACCERT_CALL(m_engine->RegisterObjectType("CTimerOneshot"   , sizeof(ha::datetime::CTimerOneshot   ), asOBJ_REF | asOBJ_GC));
  // HA_ACCERT_CALL(m_engine->RegisterObjectType("CTimerManager"   , 0, asOBJ_REF | asOBJ_NOCOUNT));

  // EDeviceType
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EDeviceType", "dtZigbee" , ha::homed::EDeviceType::dtZigbee ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EDeviceType", "dtCustom" , ha::homed::EDeviceType::dtCustom ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EDeviceType", "dtUnknown", ha::homed::EDeviceType::dtUnknown));

  // EPropertyValueType
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtEnum"    , ha::homed::EPropertyValueType::pvtEnum    ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtString"  , ha::homed::EPropertyValueType::pvtString  ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtBool"    , ha::homed::EPropertyValueType::pvtBool    ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtDouble"  , ha::homed::EPropertyValueType::pvtDouble  ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtInt"     , ha::homed::EPropertyValueType::pvtInt     ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtColor"   , ha::homed::EPropertyValueType::pvtColor   ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("EPropertyValueType", "pvtUnknown" , ha::homed::EPropertyValueType::pvtUnknown ));

  // ESunTrackerEvent
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steNadir"                           ,ha::datetime::ESunTrackerEvent::steNadir                           ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningBlueHourStart"            ,ha::datetime::ESunTrackerEvent::steMorningBlueHourStart            ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningAstronomicalTwilightStart",ha::datetime::ESunTrackerEvent::steMorningAstronomicalTwilightStart));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningAstronomicalTwilightEnd"  ,ha::datetime::ESunTrackerEvent::steMorningAstronomicalTwilightEnd  ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningNauticalTwilightStart"    ,ha::datetime::ESunTrackerEvent::steMorningNauticalTwilightStart    ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningNauticalTwilightEnd"      ,ha::datetime::ESunTrackerEvent::steMorningNauticalTwilightEnd      ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningCivilTwilightStart"       ,ha::datetime::ESunTrackerEvent::steMorningCivilTwilightStart       ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningCivilTwilightEnd"         ,ha::datetime::ESunTrackerEvent::steMorningCivilTwilightEnd         ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningBlueHourEnd"              ,ha::datetime::ESunTrackerEvent::steMorningBlueHourEnd              ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningGoldenHourStart"          ,ha::datetime::ESunTrackerEvent::steMorningGoldenHourStart          ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningSunriseStart"             ,ha::datetime::ESunTrackerEvent::steMorningSunriseStart             ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningSunriseEnd"               ,ha::datetime::ESunTrackerEvent::steMorningSunriseEnd               ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steMorningGoldenHourEnd"            ,ha::datetime::ESunTrackerEvent::steMorningGoldenHourEnd            ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steZenith"                          ,ha::datetime::ESunTrackerEvent::steZenith                          ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningGoldenHourStart"          ,ha::datetime::ESunTrackerEvent::steEveningGoldenHourStart          ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningGoldenHourEnd"            ,ha::datetime::ESunTrackerEvent::steEveningGoldenHourEnd            ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningBlueHourStart"            ,ha::datetime::ESunTrackerEvent::steEveningBlueHourStart            ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningSunsetStart"              ,ha::datetime::ESunTrackerEvent::steEveningSunsetStart              ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningSunsetEnd"                ,ha::datetime::ESunTrackerEvent::steEveningSunsetEnd                ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningBlueHourEnd"              ,ha::datetime::ESunTrackerEvent::steEveningBlueHourEnd              ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningCivilTwilightStart"       ,ha::datetime::ESunTrackerEvent::steEveningCivilTwilightStart       ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningCivilTwilightEnd"         ,ha::datetime::ESunTrackerEvent::steEveningCivilTwilightEnd         ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningNauticalTwilightStart"    ,ha::datetime::ESunTrackerEvent::steEveningNauticalTwilightStart    ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningNauticalTwilightEnd"      ,ha::datetime::ESunTrackerEvent::steEveningNauticalTwilightEnd      ));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningAstronomicalTwilightStart",ha::datetime::ESunTrackerEvent::steEveningAstronomicalTwilightStart));
  HA_ACCERT_CALL(m_engine->RegisterEnumValue("ESunTrackerEvent",  "steEveningAstronomicalTwilightEnd"  ,ha::datetime::ESunTrackerEvent::steEveningAstronomicalTwilightEnd  ));

  // CColor
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f()"                   , asFUNCTION(CreateCColorDefault)  , asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(uint8, uint8, uint8)", asFUNCTION(CreateCColorRGB), asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(const int &in)"       , asFUNCTION(CreateCColorInt)      , asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(const string &in)"    , asFUNCTION(CreateCColorString)   , asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(const CColor &in)"    , asFUNCTION(CreateCColorCopy)     , asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CColor", asBEHAVE_DESTRUCT , "void f()"                   , asFUNCTION(CColorDestructor)     , asCALL_CDECL_OBJFIRST));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CColor", "uint8 R() const"                  , asMETHODPR(ha::homed::CColor, R           , () const, unsigned char        ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CColor", "uint8 G() const"                  , asMETHODPR(ha::homed::CColor, G           , () const, unsigned char        ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CColor", "uint8 B() const"                  , asMETHODPR(ha::homed::CColor, B           , () const, unsigned char        ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CColor", "string asHexString() const"       , asMETHODPR(ha::homed::CColor, asHexString , () const, std::string          ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CColor", "int asInt() const"                , asMETHODPR(ha::homed::CColor, asInt       , () const, int                  ), asCALL_THISCALL));

  // CValue
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const string asString() const"      , asMETHODPR(ha::homed::CValue, asString     , () const, const std::string                  ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const int asInt() const"            , asMETHODPR(ha::homed::CValue, asInt        , () const, const int                          ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const double asDouble() const"      , asMETHODPR(ha::homed::CValue, asDouble     , () const, const double                       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const bool asBool() const"          , asMETHODPR(ha::homed::CValue, asBool       , () const, const bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const CColor asColor() const"       , asMETHODPR(ha::homed::CValue, asColor      , () const, const ha::homed::CColor            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "bool isString() const"              , asMETHODPR(ha::homed::CValue, isString     , () const, bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "bool isInt() const"                 , asMETHODPR(ha::homed::CValue, isInt        , () const, bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "bool isDouble() const"              , asMETHODPR(ha::homed::CValue, isDouble     , () const, bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "bool isBool() const"                , asMETHODPR(ha::homed::CValue, isBool       , () const, bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "bool isColor() const"               , asMETHODPR(ha::homed::CValue, isColor      , () const, bool                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CValue", "const uint &timestamp() const"      , asMETHODPR(ha::homed::CValue, timestamp    , () const, const ha::homed::TTimestamp& ), asCALL_THISCALL));

  RegisterVector<std::string>("CValues", "CValue", m_engine);

  // CStorage
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "uint16 count() const"              , asMETHODPR(ha::homed::CStorage, count        , () const                 , size_t                  ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "bool empty() const"                , asMETHODPR(ha::homed::CStorage, empty        , () const                 , bool                    ), asCALL_THISCALL));
  // HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "bool changed() const"              , asMETHODPR(ha::homed::CStorage, changed      , () const                 , bool                    ), asCALL_THISCALL));
  // HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "bool valueChanged()"               , asMETHODPR(ha::homed::CStorage, valueChanged , ()                       , bool                    ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "void clear()"                      , asMETHODPR(ha::homed::CStorage, clear        , ()                       , void                    ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "CValues list() const"              , asMETHODPR(ha::homed::CStorage, list         , () const                 , ha::homed::CValues      ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "const string type() const"         , asMETHODPR(ha::homed::CStorage, type         , () const                 , const std::string       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "const CValue &last() const"        , asMETHODPR(ha::homed::CStorage, last         , () const                 , const ha::homed::CValue&), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "const CValue &at(uint) const"      , asMETHODPR(ha::homed::CStorage, at           , (const size_t &) const   , const ha::homed::CValue&), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CStorage", "CProperty @property()"             , asMETHODPR(ha::homed::CStorage, property     , ()                       , ha::homed::CProperty*   ), asCALL_THISCALL));

  // CProperty
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const string &name() const"                                          , asMETHODPR(ha::homed::CProperty, name      , () const                                              , const std::string&                  ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const bool &readonly() const"                                        , asMETHODPR(ha::homed::CProperty, readonly  , () const                                              , const bool&                         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const EPropertyValueType &valueType() const"                         , asMETHODPR(ha::homed::CProperty, valueType , () const                                              , const ha::homed::EPropertyValueType&), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const CStrings &enumerate() const"                                   , asMETHODPR(ha::homed::CProperty, enumerate , () const                                              , const ha::utils::CStrings&          ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const double &min() const"                                           , asMETHODPR(ha::homed::CProperty, min       , () const                                              , const double&                       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const double &max() const"                                           , asMETHODPR(ha::homed::CProperty, max       , () const                                              , const double&                       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const double &step() const"                                          , asMETHODPR(ha::homed::CProperty, step      , () const                                              , const double&                       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "CStorage @storage()"                                                 , asMETHODPR(ha::homed::CProperty, storage   , ()                                                    , ha::homed::CStorage*                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const CValue &last() const"                                          , asMETHODPR(ha::homed::CProperty, last      , () const                                              , const ha::homed::CValue&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "const string type() const"                                           , asMETHODPR(ha::homed::CProperty, type      , () const                                              , const std::string                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "CDevice @device()"                                                   , asMETHODPR(ha::homed::CProperty, device    , ()                                                    , ha::homed::CDevice*                 ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "CEndpoint @endpoint()"                                               , asMETHODPR(ha::homed::CProperty, endpoint  , ()                                                    , ha::homed::CEndpoint*               ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void subscribe(const string &in, const string &in, const bool &in)"  , asMETHODPR(ha::homed::CProperty, subscribe , (const std::string&, const std::string&, const bool&) , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const string &in)"                                          , asMETHODPR(ha::homed::CProperty, set       , (const std::string&)                                  , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const int &in)"                                             , asMETHODPR(ha::homed::CProperty, set       , (const int&)                                          , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const double &in)"                                          , asMETHODPR(ha::homed::CProperty, set       , (const double&)                                       , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const bool &in)"                                            , asMETHODPR(ha::homed::CProperty, set       , (const bool&)                                         , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const CColor &in)"                                          , asMETHODPR(ha::homed::CProperty, set       , (const ha::homed::CColor&)                            , void                                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperty", "void set(const CValue &in)"                                          , asMETHODPR(ha::homed::CProperty, set       , (const ha::homed::CValue&)                            , void                                ), asCALL_THISCALL));

  // CProperties
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "const bool empty() const"                 , asMETHODPR(ha::homed::CProperties, empty    , () const                , const bool                    ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "CStrings list()"                          , asMETHODPR(ha::homed::CProperties, list     , ()                      , ha::utils::CStrings           ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "bool exists(const string &in)"            , asMETHODPR(ha::homed::CProperties, exists   , (const std::string &)   , bool                          ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "CProperty@ get(const string &in)"         , asMETHODPR(ha::homed::CProperties, get      , (const std::string &)   , ha::homed::CProperty*         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "CProperty@ get(const uint16 &in)"         , asMETHODPR(ha::homed::CProperties, get      , (const unsigned short &), ha::homed::CProperty*         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "uint16 size()"                            , asMETHODPR(ha::homed::CProperties, size     , ()                      , unsigned short                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "CDevice @device()"                        , asMETHODPR(ha::homed::CProperties, device   , ()                      , ha::homed::CDevice*           ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CProperties", "CEndpoint @endpoint()"                    , asMETHODPR(ha::homed::CProperties, endpoint , ()                      , ha::homed::CEndpoint*         ), asCALL_THISCALL));

  // CEndpoint
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoint", "const string &name() const"                 , asMETHODPR(ha::homed::CEndpoint, name      , () const, const std::string&       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoint", "CProperties@ properties()"                  , asMETHODPR(ha::homed::CEndpoint, properties, ()      , ha::homed::CProperties*  ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoint", "CDevice @device()"                          , asMETHODPR(ha::homed::CEndpoint, device    , ()      , ha::homed::CDevice*      ), asCALL_THISCALL));

  // CEndpoints
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "const bool empty() const"                 , asMETHODPR(ha::homed::CEndpoints, empty  , () const                , const bool                    ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "CStrings list()"                          , asMETHODPR(ha::homed::CEndpoints, list   , ()                      , ha::utils::CStrings           ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "bool exists(const string &in)"            , asMETHODPR(ha::homed::CEndpoints, exists , (const std::string &)   , bool                          ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "CEndpoint@ get(const string &in)"         , asMETHODPR(ha::homed::CEndpoints, get    , (const std::string &)   , ha::homed::CEndpoint*         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "CEndpoint@ get(const uint16 &in)"         , asMETHODPR(ha::homed::CEndpoints, get    , (const unsigned short &), ha::homed::CEndpoint*         ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "uint16 size()"                            , asMETHODPR(ha::homed::CEndpoints, size   , ()                      , unsigned short                ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CEndpoints", "CDevice @device()"                        , asMETHODPR(ha::homed::CEndpoints, device , ()                      , ha::homed::CDevice*           ), asCALL_THISCALL));

  // CDevice
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const string &name() const"                   , asMETHODPR(ha::homed::CDevice, name             , () const, const std::string&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const EDeviceType &type() const"              , asMETHODPR(ha::homed::CDevice, type             , () const, const ha::homed::EDeviceType& ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const string &firmware() const"               , asMETHODPR(ha::homed::CDevice, firmware         , () const, const std::string&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const string &manufacturerName() const"       , asMETHODPR(ha::homed::CDevice, manufacturerName , () const, const std::string&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const string &modelName() const"              , asMETHODPR(ha::homed::CDevice, modelName        , () const, const std::string&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const string &description() const"            , asMETHODPR(ha::homed::CDevice, description      , () const, const std::string&            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const bool &interviewFinished() const"        , asMETHODPR(ha::homed::CDevice, interviewFinished, () const, const bool&                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const bool &supported() const"                , asMETHODPR(ha::homed::CDevice, supported        , () const, const bool&                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const bool &active() const"                   , asMETHODPR(ha::homed::CDevice, active           , () const, const bool&                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const bool &cloud() const"                    , asMETHODPR(ha::homed::CDevice, cloud            , () const, const bool&                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "const bool &discovery() const"                , asMETHODPR(ha::homed::CDevice, discovery        , () const, const bool&                   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "CProperties@ properties()"                    , asMETHODPR(ha::homed::CDevice, properties       , ()      , ha::homed::CProperties*       ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevice", "CEndpoints@ endpoints()"                      , asMETHODPR(ha::homed::CDevice, endpoints        , ()      , ha::homed::CEndpoints*        ), asCALL_THISCALL));

  // CDevices
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "const bool empty() const"                 , asMETHODPR(ha::homed::CDevices, empty  , () const                , const bool                     ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "const EDeviceType &type() const"          , asMETHODPR(ha::homed::CDevices, type   , () const                , const ha::homed::EDeviceType&  ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "CStrings list()"                          , asMETHODPR(ha::homed::CDevices, list   , ()                      , ha::utils::CStrings            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "bool exists(const string &in)"            , asMETHODPR(ha::homed::CDevices, exists , (const std::string &)   , bool                           ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "CDevice@ get(const string &in)"           , asMETHODPR(ha::homed::CDevices, get    , (const std::string &)   , ha::homed::CDevice*            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "CDevice@ get(const uint16 &in)"           , asMETHODPR(ha::homed::CDevices, get    , (const unsigned short &), ha::homed::CDevice*            ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CDevices", "uint16 size()"                            , asMETHODPR(ha::homed::CDevices, size   , ()                      , unsigned short                 ), asCALL_THISCALL));

  // CHomed
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CDevices@ devices(const EDeviceType &in)"                                                         , asMETHODPR(ha::homed::CHomed, devices   , (const ha::homed::EDeviceType &)                                                               , ha::homed::CDevices*    ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CDevice@ device(const string &in)"                                                                , asMETHODPR(ha::homed::CHomed, device    , (const std::string &)                                                                          , ha::homed::CDevice*     ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CDevice@ device(const EDeviceType &in, const string &in)"                                         , asMETHODPR(ha::homed::CHomed, device    , (const ha::homed::EDeviceType &, const std::string &)                                          , ha::homed::CDevice*     ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CEndpoint@ endpoint(const EDeviceType &in, const string &in, const string &in)"                   , asMETHODPR(ha::homed::CHomed, endpoint  , (const ha::homed::EDeviceType &, const std::string &, const std::string &)                     , ha::homed::CEndpoint*   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CProperty@ property(const EDeviceType &in, const string &in, const string &in)"                   , asMETHODPR(ha::homed::CHomed, property  , (const ha::homed::EDeviceType &, const std::string &, const std::string &)                     , ha::homed::CProperty*   ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CHomed", "CProperty@ property(const EDeviceType &in, const string &in, const string &in, const string &in)" , asMETHODPR(ha::homed::CHomed, property  , (const ha::homed::EDeviceType &, const std::string &, const std::string &, const std::string &), ha::homed::CProperty*   ), asCALL_THISCALL));

  // CTimerContinuous
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_FACTORY, "CTimerContinuous@ f(const string &in, const string &in, const int64 &in)", asFUNCTION(CreateCTimerContinuous), asCALL_CDECL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_ADDREF , "void f()", asMETHOD(ha::datetime::CTimerContinuous, addRef ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_RELEASE, "void f()", asMETHOD(ha::datetime::CTimerContinuous, release), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_GETREFCOUNT, "int f()"       , asMETHOD(ha::datetime::CTimerContinuous, getRefCount), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_SETGCFLAG  , "void f()"      , asMETHOD(ha::datetime::CTimerContinuous, setGCFlag)  , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_GETGCFLAG  , "bool f()"      , asMETHOD(ha::datetime::CTimerContinuous, getGCFlag)  , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_ENUMREFS   , "void f(int&in)", asMETHOD(ha::datetime::CTimerContinuous, enumRefs)   , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerContinuous", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(ha::datetime::CTimerContinuous, releaseRefs), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerContinuous", "CTimerContinuous &opAssign(const CTimerContinuous &in)", asMETHODPR(ha::datetime::CTimerContinuous, operator=, (const ha::datetime::CTimerContinuous&), ha::datetime::CTimerContinuous&), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerContinuous", "void start()" , asMETHODPR(ha::datetime::CTimerContinuous, start, (), void ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerContinuous", "void stop()"  , asMETHODPR(ha::datetime::CTimerContinuous, stop , (), void ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerContinuous", "void reset()" , asMETHODPR(ha::datetime::CTimerContinuous, reset, (), void ), asCALL_THISCALL));

  // CTimerOneshot
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_FACTORY, "CTimerOneshot@ f(const string &in, const string &in, const int64 &in)", asFUNCTION(CreateCTimerOneshot), asCALL_CDECL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_ADDREF , "void f()", asMETHOD(ha::datetime::CTimerOneshot, addRef ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_RELEASE, "void f()", asMETHOD(ha::datetime::CTimerOneshot, release), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_GETREFCOUNT, "int f()"       , asMETHOD(ha::datetime::CTimerOneshot, getRefCount), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_SETGCFLAG  , "void f()"      , asMETHOD(ha::datetime::CTimerOneshot, setGCFlag)  , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_GETGCFLAG  , "bool f()"      , asMETHOD(ha::datetime::CTimerOneshot, getGCFlag)  , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_ENUMREFS   , "void f(int&in)", asMETHOD(ha::datetime::CTimerOneshot, enumRefs)   , asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectBehaviour("CTimerOneshot", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(ha::datetime::CTimerOneshot, releaseRefs), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerOneshot", "CTimerOneshot &opAssign(const CTimerOneshot &in)", asMETHODPR(ha::datetime::CTimerOneshot, operator=, (const ha::datetime::CTimerOneshot&), ha::datetime::CTimerOneshot&), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerOneshot", "void start()" , asMETHODPR(ha::datetime::CTimerOneshot, start, (), void ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerOneshot", "void stop()"  , asMETHODPR(ha::datetime::CTimerOneshot, stop , (), void ), asCALL_THISCALL));
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerOneshot", "void reset()" , asMETHODPR(ha::datetime::CTimerOneshot, reset, (), void ), asCALL_THISCALL));

  // CSunTracker
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CSunTracker", "void subscribe(const ESunTrackerEvent &in, const string &in, const string &in)"  , asMETHODPR(ha::datetime::CSunTracker, subscribe , (const ha::datetime::ESunTrackerEvent&, const std::string&, const std::string&) , void), asCALL_THISCALL));

  // CAstronomical
  HA_ACCERT_CALL(m_engine->RegisterObjectMethod("CAstronomical", "CSunTracker@ sun()" , asMETHODPR(ha::datetime::CAstronomical, sun, (), ha::datetime::CSunTracker *), asCALL_THISCALL));

}

void CScriptEnvironment::registerVariables()
{
  HA_ACCERT_CALL(m_engine->RegisterGlobalProperty("string script_name", &m_name));
  HA_ACCERT_CALL(m_engine->RegisterGlobalProperty("CLogger @logger", &m_logger));
  HA_ACCERT_CALL(m_engine->RegisterGlobalProperty("CHomed homed", &HA_ST.homed()));
  HA_ACCERT_CALL(m_engine->RegisterGlobalProperty("CAstronomical astro", &HA_ST.astro()));
}

}
}
