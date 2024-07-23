#pragma once
#include <string>
#include <filesystem>
#include <cassert>
#include <angelscript.h>

#include "scripting/helpers/clogger.h"
#include "sdk/add_on/scriptbuilder/scriptbuilder.h"

namespace ha
{
namespace scripting
{

class CScriptEnvironment
{
public:
  explicit CScriptEnvironment(const std::filesystem::path &file);
  virtual ~CScriptEnvironment();

  const std::string &name() const { return m_name; }
  const std::filesystem::path &file() const { return m_file; }

protected:
  asIScriptEngine *m_engine;
  CScriptBuilder *m_builder;
  asIScriptContext *m_context;
  // asIScriptModule *m_module;
  void registerEntities();
  bool build();

private:
  std::filesystem::path m_file;
  std::string m_name;
  ha::scripting::helpers::CLogger *m_logger;

  std::string content();
  std::string extractName(const std::filesystem::path& file);

  void messageCallback(const asSMessageInfo* msg);


  void registerBase();
  void registerLogger();
  void registerModel();
  void registerVariables();
};

#define HA_ACCERT_CALL(_what) { int r = _what; assert(r >= 0); }
#define HA_LOG_SCRIPT(_priority,_what) std::cout << "[" << file().string() << "] ["<< _priority << "] " << _what << std::endl;

}
}
