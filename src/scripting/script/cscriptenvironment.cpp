#include "scripting/script/cscriptenvironment.h"
#include <fstream>
#include <string>

#include "sdk/add_on/scriptstdstring/scriptstdstring.h"
#include "sdk/add_on/scriptarray/scriptarray.h"
#include "sdk/tests/test_feature/source/stdvector.h"

// #include "aatc.hpp"

#include "datetime/astronomical/csuntracker.h"
#include "mqtt/scripting/cmqttscriptcallback.h"
#include "homed/chomed.h"
#include "mqtt/homed/ctopic.h"
#include "utils/thread.h"
#include "utils/string.h"
#include "utils/numeric.h"
#include "utils/random.h"

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

std::string CScriptEnvironment::extractName(const std::filesystem::path & file)
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
  HA_LOG_SCRIPT(msgTypeToStr(msg->type), msg->section << " (" << msg->row << ", " << msg->col << "): " << msg->message);
}

const char *CScriptEnvironment::msgTypeToStr(const asEMsgType &type)
{
  switch (type)
  {
    case asEMsgType::asMSGTYPE_ERROR      : return "err";
    case asEMsgType::asMSGTYPE_WARNING    : return "wrn";
    case asEMsgType::asMSGTYPE_INFORMATION: return "inf";
  }
  return "unk";
}

void CScriptEnvironment::registerEntities()
{
  HA_AS_ACCERT_CALL(m_engine->SetMessageCallback(asMETHOD(CScriptEnvironment, messageCallback), this, asCALL_THISCALL));
  registerBase();
  registerLogger();
  registerContainers();
  registerEnumerations();
  registerModel();
  registerSerial();
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
  // aatc::RegisterAllContainers(m_engine);
}

void CScriptEnvironment::registerLogger()
{
  HA_AS_CLASS_MANAGED(ha::scripting::helpers, CLogger);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, dbg, void, (const std::string &in),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, nfo, void, (const std::string &in),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, wrn, void, (const std::string &in),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, err, void, (const std::string &in),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CLogger, cry, void, (const std::string &in),      );
}

void CScriptEnvironment::registerContainers()
{
  RegisterVector<std::string>("CStrings", "string", m_engine);
}

void CScriptEnvironment::registerEnumerations()
{

  // ETopic
  HA_AS_ENUM(ETopic);
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tCommand);
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tDevice );
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tEvent  );
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tExpose );
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tFd     );
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tService);
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tStatus );
  HA_AS_ENUM_VALUE(ha::mqtt, ETopic, tTd     );

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
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steNadir                           );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningBlueHourStart            );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningAstronomicalTwilightStart);
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningAstronomicalTwilightEnd  );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningNauticalTwilightStart    );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningNauticalTwilightEnd      );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningCivilTwilightStart       );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningCivilTwilightEnd         );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningBlueHourEnd              );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningGoldenHourStart          );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningSunriseStart             );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningSunriseEnd               );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steMorningGoldenHourEnd            );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steZenith                          );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningGoldenHourStart          );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningGoldenHourEnd            );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningBlueHourStart            );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningSunsetStart              );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningSunsetEnd                );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningBlueHourEnd              );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningCivilTwilightStart       );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningCivilTwilightEnd         );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningNauticalTwilightStart    );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningNauticalTwilightEnd      );
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningAstronomicalTwilightStart);
  HA_AS_ENUM_VALUE(ha::datetime, ESunTrackerEvent, steEveningAstronomicalTwilightEnd  );
}

void CScriptEnvironment::registerModel()
{
  // functions
  HA_AS_FUNCTION(ha::utils, sleep          , void                    , (const unsigned int &                                         ));
  HA_AS_FUNCTION(ha::utils, to_snake_case  , std::string             , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, to_camel_case  , std::string             , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, to_lower       , std::string             , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, to_upper       , std::string             , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, trim           , std::string             , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, contains       , bool                    , (const std::string &, const std::string &                     ));
  HA_AS_FUNCTION(ha::utils, match          , bool                    , (const std::string &, const std::string &                     ));
  HA_AS_FUNCTION(ha::utils, extract        , std::string             , (const std::string &, const std::string &, int                ));
  HA_AS_FUNCTION(ha::utils, split          , std::vector<std::string>, (const std::string &, const char &                            ));
  HA_AS_FUNCTION(ha::utils, replace        , std::string             , (const std::string &, const std::string &, const std::string &));
  HA_AS_FUNCTION(ha::utils, random_string  , std::string             , (const std::string &, const size_t                            ));
  HA_AS_FUNCTION(ha::utils, random_string  , std::string             , (const size_t                                                 ));
  HA_AS_FUNCTION(ha::utils, shuffle_strings, std::vector<std::string>, (std::vector<std::string>                                     ));
  HA_AS_FUNCTION(ha::utils, sort_strings   , std::vector<std::string>, (std::vector<std::string>                                     ));
  HA_AS_FUNCTION(ha::utils, unique_strings , std::vector<std::string>, (std::vector<std::string>                                     ));
  HA_AS_FUNCTION(ha::utils, calculate_hash , size_t                  , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, random         , int                     , (int, int                                                     ));
  HA_AS_FUNCTION(ha::utils, random         , double                  , (double, double                                               ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (float                                                        ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (double                                                       ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (int                                                          ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (unsigned int                                                 ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (short                                                        ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (unsigned short                                               ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (int64_t                                                      ));
  HA_AS_FUNCTION(ha::utils, to_string      , std::string             , (uint64_t                                                     ));
  HA_AS_FUNCTION(ha::utils, to_int         , int                     , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, to_double      , double                  , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, hex_to_int     , int                     , (const std::string &                                          ));
  HA_AS_FUNCTION(ha::utils, int_to_hex     , std::string             , (int                                                          ));

  // types
  HA_AS_STRUCT(ha::mqtt, SMqttMesssage);

  HA_AS_CLASS_VALUE(ha::scripting::helpers, CJson);

  HA_AS_CLASS_VALUE(ha::datetime, CTimeInterval   );
  HA_AS_CLASS_VALUE(ha::datetime, CDateTime       );

  HA_AS_CLASS_SMART(ha::homed   , CColor          );
  HA_AS_CLASS_SMART(ha::datetime, CTimerContinuous);
  HA_AS_CLASS_SMART(ha::datetime, CTimerOneshot   );
  HA_AS_CLASS_SMART(ha::datetime, CTimerCron      );

  HA_AS_CLASS_MANAGED(ha::homed   , CValue       );
  HA_AS_CLASS_MANAGED(ha::homed   , CStorage     );
  HA_AS_CLASS_MANAGED(ha::homed   , CProperty    );
  HA_AS_CLASS_MANAGED(ha::homed   , CProperties  );
  HA_AS_CLASS_MANAGED(ha::homed   , CEndpoint    );
  HA_AS_CLASS_MANAGED(ha::homed   , CEndpoints   );
  HA_AS_CLASS_MANAGED(ha::homed   , CDevice      );
  HA_AS_CLASS_MANAGED(ha::homed   , CDevices     );
  HA_AS_CLASS_MANAGED(ha::homed   , CInstance    );
  HA_AS_CLASS_MANAGED(ha::homed   , CInstances   );
  HA_AS_CLASS_MANAGED(ha::homed   , CHomed       );
  HA_AS_CLASS_MANAGED(ha::datetime, CSunTracker  );
  HA_AS_CLASS_MANAGED(ha::datetime, CAstronomical);
  HA_AS_CLASS_MANAGED(ha::mqtt    , CMqtt        );

  // SMqttMesssage
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::mqtt, SMqttMesssage, (), );
  HA_AS_CLASS_VALUE_DESTRUCTOR(ha::mqtt, SMqttMesssage);
  HA_AS_STRUCT_FIELD(ha::mqtt, SMqttMesssage, std::string, topic);
  HA_AS_STRUCT_FIELD(ha::mqtt, SMqttMesssage, std::string, payload);

  // CJson
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::scripting::helpers, CJson, (                                     ),       );
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::scripting::helpers, CJson, (const std::string &                  ), String);
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::scripting::helpers, CJson, (const ha::scripting::helpers::CJson &), Copy  );
  HA_AS_CLASS_VALUE_DESTRUCTOR(ha::scripting::helpers, CJson);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator=     , ha::scripting::helpers::CJson &, (const ha::scripting::helpers::CJson &),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator=     , ha::scripting::helpers::CJson &, (const std::string &                  ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator=     , ha::scripting::helpers::CJson &, (const int &                          ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator=     , ha::scripting::helpers::CJson &, (const double &                       ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator=     , ha::scripting::helpers::CJson &, (const bool &                         ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator[]    , ha::scripting::helpers::CJson &, (int                                  ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator[]    , ha::scripting::helpers::CJson &, (const std::string &                  ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator<     , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator<=    , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator>     , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator>=    , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator==    , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, operator!=    , bool                           , (const ha::scripting::helpers::CJson &), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isMember      , bool                           , (const std::string &                  ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, getMemberNames, ha::utils::CStrings            , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, toStyledString, std::string                    , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, removeMember  , void                           , (const std::string &                  ),      );
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isNull        , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isBool        , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isInt         , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isDouble      , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isString      , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isArray       , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, isObject      , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, asString      , std::string                    , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, asInt         , int                            , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, asDouble      , double                         , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, asBool        , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, empty         , bool                           , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, size          , unsigned int                   , (                                     ), const);
  HA_AS_CLASS_METHOD(ha::scripting::helpers, CJson, clear         , void                           , (                                     ),      );

  // CColor
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::homed, CColor, (                                           ));
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::homed, CColor, (unsigned char, unsigned char, unsigned char));
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::homed, CColor, (const int &                                ));
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::homed, CColor, (const std::string &                        ));
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::homed, CColor, (const ha::homed::CColor &                  ));
  HA_AS_CLASS_METHOD(ha::homed, CColor, R          , unsigned char, (), const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, G          , unsigned char, (), const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, B          , unsigned char, (), const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, asHexString, std::string  , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CColor, asInt      , int          , (), const);

  // CValue
  HA_AS_CLASS_METHOD(ha::homed, CValue, asString , const std::string &            , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asInt    , const int &                    , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asDouble , const double &                 , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asBool   , const bool &                   , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, asColor  , const ha::homed::CColor &      , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isString , bool                           , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isInt    , bool                           , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isDouble , bool                           , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isBool   , bool                           , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, isColor  , bool                           , (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, timestamp, const ha::datetime::CDateTime &, (), const);
  HA_AS_CLASS_METHOD(ha::homed, CValue, storage  , ha::homed::CStorage *          , (), const);

  RegisterVector<ha::homed::CValue>("CValues", "CValue", m_engine);

  // CStorage
  HA_AS_CLASS_METHOD(ha::homed, CStorage, count   , size_t                   , (              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, empty   , bool                     , (              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, clear   , void                     , (              ),      );
  HA_AS_CLASS_METHOD(ha::homed, CStorage, list    , ha::homed::CValues       , (              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, type    , const std::string        , (              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, last    , const ha::homed::CValue &, (              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, at      , const ha::homed::CValue &, (const size_t &), const);
  HA_AS_CLASS_METHOD(ha::homed, CStorage, property, ha::homed::CProperty *   , (              ), const);

  // CProperty
  HA_AS_CLASS_METHOD(ha::homed, CProperty, name         , const std::string &                  , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, readonly     , const bool &                         , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, valueType    , const ha::homed::EPropertyValueType &, (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, enumerate    , const ha::utils::CStrings &          , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, min          , const double &                       , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, max          , const double &                       , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, step         , const double &                       , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, storage      , ha::homed::CStorage *                , (                                                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, last         , const ha::homed::CValue &            , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, type         , const std::string                    , (                                                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperty, device       , ha::homed::CDevice *                 , (                                                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, endpoint     , ha::homed::CEndpoint *               , (                                                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, subscribe    , void                                 , (const std::string &, const std::string &, const bool &),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, unsubscribe  , void                                 , (const std::string &, const std::string &              ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set          , void                                 , (const std::string &                                   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set          , void                                 , (const int &                                           ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set          , void                                 , (const double &                                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set          , void                                 , (const bool &                                          ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, set          , void                                 , (const ha::homed::CColor &                             ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, setIfNotEqual, void                                 , (const std::string &                                   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, setIfNotEqual, void                                 , (const int &                                           ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, setIfNotEqual, void                                 , (const double &                                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, setIfNotEqual, void                                 , (const bool &                                          ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperty, setIfNotEqual, void                                 , (const ha::homed::CColor &                             ),      );

  // CProperties
  HA_AS_CLASS_METHOD(ha::homed, CProperties, empty   , const bool            , (                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CProperties, list    , ha::utils::CStrings   , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, exists  , bool                  , (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, get     , ha::homed::CProperty *, (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, get     , ha::homed::CProperty *, (const unsigned short &),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, size    , unsigned short        , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, device  , ha::homed::CDevice *  , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CProperties, endpoint, ha::homed::CEndpoint *, (                      ),      );

  // CEndpoint
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, name      , const std::string &     , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, properties, ha::homed::CProperties *, (                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, device    , ha::homed::CDevice *    , (                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoint, topicPath , std::string             , (const ha::mqtt::ETopic &),      );

  // CEndpoints
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, empty   , const bool            , (                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, list    , ha::utils::CStrings   , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, exists  , bool                  , (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, get     , ha::homed::CEndpoint *, (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, get     , ha::homed::CEndpoint *, (const unsigned short &),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, size    , unsigned short        , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CEndpoints, device  , ha::homed::CDevice *  , (                      ),      );

  // CDevice
  HA_AS_CLASS_METHOD(ha::homed, CDevice, name             , const std::string &           , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, type             , const ha::homed::EDeviceType &, (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, firmware         , const std::string &           , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, manufacturerName , const std::string &           , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, modelName        , const std::string &           , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, description      , const std::string &           , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, interviewFinished, const bool &                  , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, supported        , const bool &                  , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, active           , const bool &                  , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, cloud            , const bool &                  , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, discovery        , const bool &                  , (                        ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevice, properties       , ha::homed::CProperties *      , (                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevice, endpoints        , ha::homed::CEndpoints *       , (                        ),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevice, topicPath        , std::string                   , (const ha::mqtt::ETopic &),      );

  // CDevices
  HA_AS_CLASS_METHOD(ha::homed, CDevices, empty   , const bool                    , (                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevices, type    , const ha::homed::EDeviceType &, (                      ), const);
  HA_AS_CLASS_METHOD(ha::homed, CDevices, list    , ha::utils::CStrings           , (                      ),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, exists  , bool                          , (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, get     , ha::homed::CDevice *          , (const std::string &   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, get     , ha::homed::CDevice *          , (const unsigned short &),      );
  HA_AS_CLASS_METHOD(ha::homed, CDevices, size    , unsigned short                , (                      ),      );

  // CInstance
  HA_AS_CLASS_METHOD(ha::homed, CInstance, name     , const std::string & , (                              ), const);
  HA_AS_CLASS_METHOD(ha::homed, CInstance, devices  , ha::homed::CDevices *, (const ha::homed::EDeviceType &),      );

  // CInstances
  HA_AS_CLASS_METHOD(ha::homed, CInstances, empty   , const bool                    , (                                                   ), const);
  HA_AS_CLASS_METHOD(ha::homed, CInstances, list    , ha::utils::CStrings           , (                                                   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, exists  , bool                          , (const std::string &                                ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, get     , ha::homed::CInstance *        , (const std::string &                                ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, get     , ha::homed::CInstance *        , (const unsigned short &                             ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, size    , unsigned short                , (                                                   ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, device  , ha::homed::CDevice *          , (const std::string &                                ),      );
  HA_AS_CLASS_METHOD(ha::homed, CInstances, device  , ha::homed::CDevice *          , (const ha::homed::EDeviceType &, const std::string &),      );

  // CHomed
  // HA_AS_CLASS_METHOD(ha::homed, CHomed, devices , ha::homed::CDevices * , (const ha::homed::EDeviceType &                                                               ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, instances, ha::homed::CInstances *, (                                                                                             ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, instance , ha::homed::CInstance * , (const std::string &                                                                          ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, device   , ha::homed::CDevice *   , (const std::string &                                                                          ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, device   , ha::homed::CDevice *   , (const ha::homed::EDeviceType &, const std::string &                                          ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, endpoint , ha::homed::CEndpoint * , (const ha::homed::EDeviceType &, const std::string &, const std::string &                     ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, property , ha::homed::CProperty * , (const ha::homed::EDeviceType &, const std::string &, const std::string &                     ),      );
  HA_AS_CLASS_METHOD(ha::homed, CHomed, property , ha::homed::CProperty * , (const ha::homed::EDeviceType &, const std::string &, const std::string &, const std::string &),      );

  // CTimerContinuous
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::datetime, CTimerContinuous, (const std::string &, const std::string &, const int64_t &));
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, operator=, ha::datetime::CTimerContinuous &, (const ha::datetime::CTimerContinuous &),     );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, start, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, stop , void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, reset, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, timeInterval, const ha::datetime::CTimeInterval&, (), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimerContinuous, nextTrigger, const ha::datetime::CDateTime&, (), const);

  // CTimerOneshot
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::datetime, CTimerOneshot, (const std::string &, const std::string &, const int64_t &));
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, operator=, ha::datetime::CTimerOneshot &, (const ha::datetime::CTimerOneshot &),     );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, start, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, stop , void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, reset, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, timeInterval, const ha::datetime::CTimeInterval&, (), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimerOneshot, nextTrigger, const ha::datetime::CDateTime&, (), const);

  // CTimerCron
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::datetime, CTimerCron, (const std::string &, const std::string &, const std::string &));
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, operator=, ha::datetime::CTimerCron &, (const ha::datetime::CTimerCron &),     );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, start, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, stop , void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, reset, void, (),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, timeInterval, const ha::datetime::CTimeInterval&, (), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimerCron, nextTrigger, const ha::datetime::CDateTime&, (), const);

  // CDateTime
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CDateTime, (                                 ),       );
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CDateTime, (int64_t                          ), TS    );
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CDateTime, (const std::string &              ), String);
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CDateTime, (int, int, int, int, int, int, int), Parts );
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CDateTime, (const ha::datetime::CDateTime&   ), Copy  );
  HA_AS_CLASS_VALUE_DESTRUCTOR(ha::datetime, CDateTime);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, asString       , std::string, (const std::string &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, asString       , std::string, (                   ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, asUnixTimestamp, std::time_t, (                   ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator= , ha::datetime::CDateTime &, (const ha::datetime::CDateTime &    ),      );
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator+ , ha::datetime::CDateTime  , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator- , ha::datetime::CDateTime  , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator< , bool                     , (const ha::datetime::CDateTime &    ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator<=, bool                     , (const ha::datetime::CDateTime &    ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator> , bool                     , (const ha::datetime::CDateTime &    ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator>=, bool                     , (const ha::datetime::CDateTime &    ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator==, bool                     , (const ha::datetime::CDateTime &    ), const);
  HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator!=, bool                     , (const ha::datetime::CDateTime &    ), const);
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator< , bool                     , (ha::datetime::CDateTime            ),      );
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator<=, bool                     , (ha::datetime::CDateTime            ),      );
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator> , bool                     , (ha::datetime::CDateTime            ),      );
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator>=, bool                     , (ha::datetime::CDateTime            ),      );
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator==, bool                     , (ha::datetime::CDateTime            ),      );
  // HA_AS_CLASS_METHOD(ha::datetime, CDateTime, operator!=, bool                     , (ha::datetime::CDateTime            ),      );

  // CTimeInterval
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CTimeInterval, (double                               ), );
  HA_AS_CLASS_VALUE_CONSTRUCTOR(ha::datetime, CTimeInterval, (const ha::datetime::CTimeInterval&   ), Copy);
  HA_AS_CLASS_VALUE_DESTRUCTOR(ha::datetime, CTimeInterval);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, asString, std::string, (), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, seconds , double     , (), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator= , ha::datetime::CTimeInterval &, (const ha::datetime::CTimeInterval &),      );
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator+ , ha::datetime::CTimeInterval  , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator- , ha::datetime::CTimeInterval  , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator< , bool                         , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator<=, bool                         , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator> , bool                         , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator>=, bool                         , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator==, bool                         , (const ha::datetime::CTimeInterval &), const);
  HA_AS_CLASS_METHOD(ha::datetime, CTimeInterval, operator!=, bool                         , (const ha::datetime::CTimeInterval &), const);

  // CSunTracker
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, subscribe   , void                           , (const std::string &, const std::string &, const ha::datetime::ESunTrackerEvent &),      );
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, getEventTime, const ha::datetime::CDateTime &, (const ha::datetime::ESunTrackerEvent &                                          ),      );
  HA_AS_CLASS_METHOD(ha::datetime, CSunTracker, circumpolar , bool                           , (                                                                                ),      );

  // CAstronomical
  HA_AS_CLASS_METHOD(ha::datetime, CAstronomical, sun, ha::datetime::CSunTracker *, (),      );

  // CMqtt
  HA_AS_CLASS_METHOD(ha::mqtt, CMqtt, subscribe, void, (const std::string &, const std::string &, const std::string &),      );
  HA_AS_CLASS_METHOD(ha::mqtt, CMqtt, publish  , void, (const std::string &, const std::string &                     ),      );
}

void CScriptEnvironment::registerSerial()
{
  // EBaudRate
  HA_AS_ENUM(EBaudRate);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br0      );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br50     );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br75     );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br110    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br134    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br150    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br200    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br300    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br600    );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br1200   );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br1800   );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br2400   );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br4800   );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br9600   );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br19200  );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br38400  );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br57600  );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br115200 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br230400 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br460800 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br500000 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br576000 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br921600 );
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br1000000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br1152000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br1500000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br2000000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br2500000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br3000000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br3500000);
  HA_AS_ENUM_VALUE(ha::serial, EBaudRate, br4000000);

  // EParity
  HA_AS_ENUM(EParity);
  HA_AS_ENUM_VALUE(ha::serial, EParity, pNone);
  HA_AS_ENUM_VALUE(ha::serial, EParity, pOdd );
  HA_AS_ENUM_VALUE(ha::serial, EParity, pEven);

  // EStopBits
  HA_AS_ENUM(EStopBits);
  HA_AS_ENUM_VALUE(ha::serial, EStopBits, sbOne);
  HA_AS_ENUM_VALUE(ha::serial, EStopBits, sbTwo);

  // EFlowControl
  HA_AS_ENUM(EFlowControl);
  HA_AS_ENUM_VALUE(ha::serial, EFlowControl, fcNone);
  HA_AS_ENUM_VALUE(ha::serial, EFlowControl, fcHard);
  HA_AS_ENUM_VALUE(ha::serial, EFlowControl, fcSoft);

  // EBitsPerByte
  HA_AS_ENUM(EBitsPerByte);
  HA_AS_ENUM_VALUE(ha::serial, EBitsPerByte, bpb5);
  HA_AS_ENUM_VALUE(ha::serial, EBitsPerByte, bpb6);
  HA_AS_ENUM_VALUE(ha::serial, EBitsPerByte, bpb7);
  HA_AS_ENUM_VALUE(ha::serial, EBitsPerByte, bpb8);

  HA_AS_ENUM(ECommandEndl);
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceCR  );
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceLF  );
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceCRLF);
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceSUB );
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceNULL);
  HA_AS_ENUM_VALUE(ha::serial, ECommandEndl, ceETX );

  // CSerialCommunication
  HA_AS_CLASS_SMART(ha::serial, CSerialCommunication);
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::serial, CSerialCommunication, (const std::string &, ha::serial::EBaudRate, ha::serial::EParity, ha::serial::EStopBits, ha::serial::EFlowControl, ha::serial::EBitsPerByte));
  HA_AS_CLASS_METHOD(ha::serial, CSerialCommunication, send   , std::string, (const std::string &, const ha::serial::ECommandEndl &, int                ),      );
  HA_AS_CLASS_METHOD(ha::serial, CSerialCommunication, send   , std::string, (const std::string &, const ha::serial::ECommandEndl &, const std::string &),      );
  HA_AS_CLASS_METHOD(ha::serial, CSerialCommunication, query  , std::string, (const std::string &, const std::string &                                  ),      );
  HA_AS_CLASS_METHOD(ha::serial, CSerialCommunication, isOpen , bool       , (                                                                          ), const);
  HA_AS_CLASS_METHOD(ha::serial, CSerialCommunication, isReady, bool       , (                                                                          ),      );

  // CSerialWatcher
  HA_AS_CLASS_SMART(ha::serial, CSerialWatcher);
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::serial, CSerialWatcher, (const std::string &, ha::serial::EBaudRate, ha::serial::EParity, ha::serial::EStopBits, ha::serial::EFlowControl, ha::serial::EBitsPerByte));
  HA_AS_CLASS_METHOD(ha::serial, CSerialWatcher, start , void, (),      );
  HA_AS_CLASS_METHOD(ha::serial, CSerialWatcher, stop  , void, (),      );
  HA_AS_CLASS_METHOD(ha::serial, CSerialWatcher, isOpen, bool, (), const);

  // CGsm
  HA_AS_CLASS_SMART(ha::serial::device, CGsm);
  HA_AS_CLASS_SMART_CONSTRUCTOR(ha::serial::device, CGsm, (const std::string &, ha::serial::EBaudRate));
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, isOpen            , bool                    , (                                        ), const);
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, isReady           , bool                    , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, ready             , bool                    , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, signalLevel       , int                     , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, operatorName      , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, firmwareVersion   , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, adapterName       , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, imei              , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, imsi              , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, info              , std::string             , (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, ussd              , std::string             , (const std::string &                     ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, supportedEncodings, std::vector<std::string>, (                                        ),      );
  HA_AS_CLASS_METHOD(ha::serial::device, CGsm, sendSms           , bool                    , (const std::string &, const std::string &),      );

}

void CScriptEnvironment::registerVariables()
{
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("string script_name"   , &m_name       ));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CLogger       @logger", &m_logger     ));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CHomed        homed"  , HA_ST->homed()));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CAstronomical astro"  , HA_ST->astro()));
  HA_AS_ACCERT_CALL(m_engine->RegisterGlobalProperty("CMqtt         mqtt"   , HA_ST->mqtt ()));
}

}
}
