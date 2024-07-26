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
#include "cscriptenvironment.h"
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
  std::filesystem::path relative_path = std::filesystem::relative(file, HA_ST->config()->scriptingLocation());
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
  HA_AS_ACCERT_CALL(m_engine->SetMessageCallback(asMETHOD(CScriptEnvironment, messageCallback), this, asCALL_THISCALL));
  registerBase();
  registerLogger();
  registerEnumerations();
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
  HA_AS_CLASS_MANAGED(ha::scripting::helpers, CLogger);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, dbg, void, (const std::string &), "void", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, nfo, void, (const std::string &), "void", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, wrn, void, (const std::string &), "void", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, err, void, (const std::string &), "void", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, cry, void, (const std::string &), "void", "(const string &in)",      );
}

void CScriptEnvironment::registerEnumerations()
{
  // EDeviceType
  HA_AS_ENUM(EDeviceType);
  HA_AS_ENUM_VALUE(ha::homed, EDeviceType, dtZigbee );
  HA_AS_ENUM_VALUE(ha::homed, EDeviceType, dtCustom );
  HA_AS_ENUM_VALUE(ha::homed, EDeviceType, dtUnknown);

  // EPropertyValueType
  HA_AS_ENUM(EPropertyValueType);
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtEnum   );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtString );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtBool   );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtDouble );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtInt    );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtColor  );
  HA_AS_ENUM_VALUE(ha::homed, EPropertyValueType, pvtUnknown);

  // ESunTrackerEvent
  HA_AS_ENUM(ESunTrackerEvent);
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steNadir                           );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningBlueHourStart            );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningAstronomicalTwilightStart);
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningAstronomicalTwilightEnd  );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningNauticalTwilightStart    );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningNauticalTwilightEnd      );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningCivilTwilightStart       );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningCivilTwilightEnd         );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningBlueHourEnd              );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningGoldenHourStart          );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningSunriseStart             );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningSunriseEnd               );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steMorningGoldenHourEnd            );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steZenith                          );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningGoldenHourStart          );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningGoldenHourEnd            );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningBlueHourStart            );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningSunsetStart              );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningSunsetEnd                );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningBlueHourEnd              );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningCivilTwilightStart       );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningCivilTwilightEnd         );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningNauticalTwilightStart    );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningNauticalTwilightEnd      );
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningAstronomicalTwilightStart);
  HA_AS_ENUM_VALUE(ha::datetime,  ESunTrackerEvent, steEveningAstronomicalTwilightEnd  );
}
void CScriptEnvironment::registerModel()
{
  // types

  HA_AS_CLASS_STANDALONE(ha::homed   , CColor          );
  HA_AS_CLASS_STANDALONE(ha::datetime, CTimeInterval   );
  HA_AS_CLASS_STANDALONE(ha::datetime, CDateTime       );
  HA_AS_CLASS_STANDALONE(ha::datetime, CTimerContinuous);
  HA_AS_CLASS_STANDALONE(ha::datetime, CTimerOneshot   );

  HA_AS_CLASS_MANAGED(ha::homed   , CValue       );
  HA_AS_CLASS_MANAGED(ha::homed   , CStorage     );
  HA_AS_CLASS_MANAGED(ha::homed   , CProperty    );
  HA_AS_CLASS_MANAGED(ha::homed   , CProperties  );
  HA_AS_CLASS_MANAGED(ha::homed   , CEndpoint    );
  HA_AS_CLASS_MANAGED(ha::homed   , CEndpoints   );
  HA_AS_CLASS_MANAGED(ha::homed   , CDevice      );
  HA_AS_CLASS_MANAGED(ha::homed   , CDevices     );
  HA_AS_CLASS_MANAGED(ha::homed   , CHomed       );
  HA_AS_CLASS_MANAGED(ha::datetime, CSunTracker  );
  HA_AS_CLASS_MANAGED(ha::datetime, CAstronomical);

  // CColor
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::homed, CColor, "()"                   , (                                           ));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::homed, CColor, "(uint8, uint8, uint8)", (unsigned char, unsigned char, unsigned char));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::homed, CColor, "(const int &in)"      , (const int &                                ));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::homed, CColor, "(const string &in)"   , (const std::string &                        ));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::homed, CColor, "(const CColor &in)"   , (const ha::homed::CColor &                  ));
  HA_AS_CLASS_METHOD(ha::homed, CColor, R          , unsigned char, (), "uint8" , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, G          , unsigned char, (), "uint8" , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, B          , unsigned char, (), "uint8" , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, asHexString, std::string  , (), "string", "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, asInt      , int          , (), "int"   , "()", const);

  // CValue
  HA_AS_CLASS_METHOD(ha::homed, CValue, asString , const std::string             , (), "const string"    , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asInt    , const int                     , (), "const int"       , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asDouble , const double                  , (), "const double"    , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asBool   , const bool                    , (), "const bool"      , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asColor  , const ha::homed::CColor       , (), "const CColor @"  , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isString , bool                          , (), "bool"            , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isInt    , bool                          , (), "bool"            , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isDouble , bool                          , (), "bool"            , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isBool   , bool                          , (), "bool"            , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isColor  , bool                          , (), "bool"            , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, timestamp, const ha::datetime::CDateTime&, (), "const CDateTime&", "()", const);

  RegisterVector<std::string>("CValues", "CValue", m_engine);

  // CStorage
  HA_AS_CLASS_METHOD(ha::homed, CStorage, count   , size_t                  , (              ), "uint16"     , "()"    , const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, empty   , bool                    , (              ), "bool"       , "()"    , const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, clear   , void                    , (              ), "void"       , "()"    ,      );
  HA_AS_CLASS_METHOD(ha::homed, CStorage, list    , ha::homed::CValues      , (              ), "CValues"    , "()"    , const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, type    , const std::string       , (              ), "string"     , "()"    , const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, last    , const ha::homed::CValue&, (              ), "CValue &"   , "()"    , const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, at      , const ha::homed::CValue&, (const size_t &), "CValue &"   , "(uint)", const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, property, ha::homed::CProperty*   , (              ), "CProperty @", "()"    ,      );

  // CProperty
  HA_AS_CLASS_METHOD(ha::homed, CProperty, name     , const std::string&                  , (                                                   ), "const string &"            , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, readonly , const bool&                         , (                                                   ), "const bool &"              , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, valueType, const ha::homed::EPropertyValueType&, (                                                   ), "const EPropertyValueType &", "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, enumerate, const ha::utils::CStrings&          , (                                                   ), "const CStrings &"          , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, min      , const double&                       , (                                                   ), "const double &"            , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, max      , const double&                       , (                                                   ), "const double &"            , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, step     , const double&                       , (                                                   ), "const double &"            , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, storage  , ha::homed::CStorage*                , (                                                   ), "CStorage @"                , "()"                                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, last     , const ha::homed::CValue&            , (                                                   ), "const CValue &"            , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, type     , const std::string                   , (                                                   ), "const string"              , "()"                                                  , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, device   , ha::homed::CDevice*                 , (                                                   ), "CDevice @"                 , "()"                                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, endpoint , ha::homed::CEndpoint*               , (                                                   ), "CEndpoint @"               , "()"                                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, subscribe, void                                , (const std::string&, const std::string&, const bool&), "void"                      , "(const string &in, const string &in, const bool &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const std::string&                                 ), "void"                      , "(const string &in)"                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const int&                                         ), "void"                      , "(const int &in)"                                     ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const double&                                      ), "void"                      , "(const double &in)"                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const bool&                                        ), "void"                      , "(const bool &in)"                                    ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const ha::homed::CColor&                           ), "void"                      , "(const CColor &in)"                                  ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set      , void                                , (const ha::homed::CValue&                           ), "void"                      , "(const CValue &in)"                                  ,      );

  // CProperties
  HA_AS_CLASS_METHOD(ha::homed, CProperties, empty   , const bool           , (                      ), "const bool" , "()"                , const);
  HA_AS_CLASS_METHOD(ha::homed, CProperties, list    , ha::utils::CStrings  , (                      ), "CStrings"   , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, exists  , bool                 , (const std::string &   ), "bool"       , "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, get     , ha::homed::CProperty*, (const std::string &   ), "CProperty @", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, get     , ha::homed::CProperty*, (const unsigned short &), "CProperty @", "(const uint16 &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, size    , unsigned short       , (                      ), "uint16"     , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, device  , ha::homed::CDevice*  , (                      ), "CDevice @"  , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, endpoint, ha::homed::CEndpoint*, (                      ), "CEndpoint @", "()"                ,      );

  // CEndpoint
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, name      , const std::string&     , (), "const string &", "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, properties, ha::homed::CProperties*, (), "CProperties @" , "()",      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, device    , ha::homed::CDevice*    , (), "CDevice @"     , "()",      );

  // CEndpoints
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, empty   , const bool           , (                      ), "const bool" , "()"                , const);
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, list    , ha::utils::CStrings  , (                      ), "CStrings"   , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, exists  , bool                 , (const std::string &   ), "bool"       , "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, get     , ha::homed::CEndpoint*, (const std::string &   ), "CEndpoint @", "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, get     , ha::homed::CEndpoint*, (const unsigned short &), "CEndpoint @", "(const uint16 &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, size    , unsigned short       , (                      ), "uint16"     , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, device  , ha::homed::CDevice*  , (                      ), "CDevice @"  , "()"                ,      );

  // CDevice
  HA_AS_CLASS_METHOD(ha::homed, CDevice, name             , const std::string&           , (), "const string &"        , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, type             , const ha::homed::EDeviceType&, (), "const EDeviceType &"   , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, firmware         , const std::string&           , (), "const string &"        , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, manufacturerName , const std::string&           , (), "const string &"        , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, modelName        , const std::string&           , (), "const string &"        , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, description      , const std::string&           , (), "const string &"        , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, interviewFinished, const bool&                  , (), "const bool &"          , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, supported        , const bool&                  , (), "const bool &"          , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, active           , const bool&                  , (), "const bool &"          , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, cloud            , const bool&                  , (), "const bool &"          , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, discovery        , const bool&                  , (), "const bool &"          , "()", const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, properties       , ha::homed::CProperties*      , (), "CProperties @"         , "()",      );
  HA_AS_CLASS_METHOD(ha::homed, CDevice, endpoints        , ha::homed::CEndpoints*       , (), "CEndpoints @"          , "()",      );

  // CDevices
  HA_AS_CLASS_METHOD(ha::homed, CDevices, empty   , const bool                   , (                      ), "const bool"         , "()"                , const);
  HA_AS_CLASS_METHOD(ha::homed, CDevices, type    , const ha::homed::EDeviceType&, (                      ), "const EDeviceType &", "()"                , const);
  HA_AS_CLASS_METHOD(ha::homed, CDevices, list    , ha::utils::CStrings          , (                      ), "CStrings"           , "()"                ,      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, exists  , bool                         , (const std::string &   ), "bool"               , "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, get     , ha::homed::CDevice*          , (const std::string &   ), "CDevice @"          , "(const string &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, get     , ha::homed::CDevice*          , (const unsigned short &), "CDevice @"          , "(const uint16 &in)",      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, size    , unsigned short               , (                      ), "uint16"             , "()"                ,      );

  // CHomed
  HA_AS_CLASS_METHOD(ha::homed, CHomed, devices , ha::homed::CDevices* , (const ha::homed::EDeviceType &                                                               ), "CDevices @" , "(const EDeviceType &in)                                                      ",      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, device  , ha::homed::CDevice*  , (const std::string &                                                                          ), "CDevice @"  , "(const string &in)                                                           ",      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, device  , ha::homed::CDevice*  , (const ha::homed::EDeviceType &, const std::string &                                          ), "CDevice @"  , "(const EDeviceType &in, const string &in)                                    ",      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, endpoint, ha::homed::CEndpoint*, (const ha::homed::EDeviceType &, const std::string &, const std::string &                     ), "CEndpoint @", "(const EDeviceType &in, const string &in, const string &in)                  ",      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, property, ha::homed::CProperty*, (const ha::homed::EDeviceType &, const std::string &, const std::string &                     ), "CProperty @", "(const EDeviceType &in, const string &in, const string &in)                  ",      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, property, ha::homed::CProperty*, (const ha::homed::EDeviceType &, const std::string &, const std::string &, const std::string &), "CProperty @", "(const EDeviceType &in, const string &in, const string &in, const string &in)",      );

  // CTimerContinuous
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CTimerContinuous, "(const string &in, const string &in, const int64 &in)", (const std::string &, const std::string &, const int64_t &));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerContinuous", "CTimerContinuous &opAssign(const CTimerContinuous &in)", asMETHODPR(ha::datetime::CTimerContinuous, operator=, (const ha::datetime::CTimerContinuous&), ha::datetime::CTimerContinuous&), asCALL_THISCALL));
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, start, void, (), "void", "()",      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, stop , void, (), "void", "()",      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, reset, void, (), "void", "()",      );

  // CTimerOneshot
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CTimerOneshot, "(const string &in, const string &in, const int64 &in)", (const std::string &, const std::string &, const int64_t &));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimerOneshot", "CTimerOneshot &opAssign(const CTimerOneshot &in)", asMETHODPR(ha::datetime::CTimerOneshot, operator=, (const ha::datetime::CTimerOneshot&), ha::datetime::CTimerOneshot&), asCALL_THISCALL));
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, start, void, (), "void", "()",      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, stop , void, (), "void", "()",      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, reset, void, (), "void", "()",      );

  // CDateTime
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CDateTime, "()"                                 , (                                 ));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CDateTime, "(int)"                              , (int64_t                          ));
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CDateTime, "(int, int, int, int, int, int, int)", (int, int, int, int, int, int, int));
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, asString       , std::string, (const std::string &), "string", "(const string &in)", const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, asUnixTimestamp, std::time_t, (                   ), "int64" , "()"                , const);
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "CDateTime@ opAdd(const CTimeInterval &in) const", asMETHOD(ha::datetime::CDateTime, operator+), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "CDateTime@ opSub(const CTimeInterval &in) const", asMETHOD(ha::datetime::CDateTime, operator-), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opLess(const CDateTime &in) const"         , asMETHOD(ha::datetime::CDateTime, operator<), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opLessEqual(const CDateTime &in) const"    , asMETHOD(ha::datetime::CDateTime, operator<=), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opGreater(const CDateTime &in) const"      , asMETHOD(ha::datetime::CDateTime, operator>), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opGreaterEqual(const CDateTime &in) const" , asMETHOD(ha::datetime::CDateTime, operator>=), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opEquals(const CDateTime &in) const"       , asMETHOD(ha::datetime::CDateTime, operator==), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CDateTime", "bool opNotEquals(const CDateTime &in) const"    , asMETHOD(ha::datetime::CDateTime, operator!=), asCALL_THISCALL));

  // CTimeInterval
  HA_AS_CLASS_FACTORY_CONSTRUCTOR(ha::datetime, CTimeInterval, "(double)", (double));
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, asString, std::string, (), "string", "()", const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, seconds , double     , (), "double", "()", const);

  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "CTimeInterval@ opAdd(const CTimeInterval &in) const", asMETHOD(ha::datetime::CTimeInterval, operator+), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "CTimeInterval@ opSub(const CTimeInterval &in) const", asMETHOD(ha::datetime::CTimeInterval, operator-), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opLess(const CTimeInterval &in) const"        , asMETHOD(ha::datetime::CTimeInterval, operator<), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opLessEqual(const CTimeInterval &in) const"   , asMETHOD(ha::datetime::CTimeInterval, operator<=), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opGreater(const CTimeInterval &in) const"     , asMETHOD(ha::datetime::CTimeInterval, operator>), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opGreaterEqual(const CTimeInterval &in) const", asMETHOD(ha::datetime::CTimeInterval, operator>=), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opEquals(const CTimeInterval &in) const"      , asMETHOD(ha::datetime::CTimeInterval, operator==), asCALL_THISCALL));
  HA_AS_ACCERT_CALL(m_engine->RegisterObjectMethod("CTimeInterval", "bool opNotEquals(const CTimeInterval &in) const"   , asMETHOD(ha::datetime::CTimeInterval, operator!=), asCALL_THISCALL));

  // CSunTracker
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, subscribe   , void                     , (const ha::datetime::ESunTrackerEvent&, const std::string&, const std::string&), "void"       , "(const ESunTrackerEvent &in, const string &in, const string &in)",      );
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, getEventTime, ha::datetime::CDateTime *, (const ha::datetime::ESunTrackerEvent&                                        ), "CDateTime @", "(const ESunTrackerEvent &in)"                                    ,      );
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, circumpolar , bool                     , (                                                                             ), "bool"       , "()"                                                              ,      );

  // CAstronomical
  HA_AS_CLASS_METHOD(ha::datetime, CAstronomical, sun, ha::datetime::CSunTracker *, (), "CSunTracker @", "()",      );
}

void CScriptEnvironment::registerVariables()
{
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("string script_name"   , &m_name));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CLogger       @logger", &m_logger));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CHomed        homed" , HA_ST->homed()));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CAstronomical astro" , HA_ST->astro()));
}

}
}
