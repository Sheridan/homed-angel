#include "scripting/cscript.h"
// #include <scriptbuilder/scriptbuilder.h>
#include <fstream>
#include <string>
#include <cassert>
#include "st.h"
#include "cscript.h"

namespace ha
{
namespace scripting
{

// CScript::CScript(const std::filesystem::path &file)
//   : m_file(file),
//     m_name(m_file.stem().string()),
//     m_context(nullptr),
//     m_module(nullptr)
// {
//   // initialize();
// }

// CScript::~CScript()
// {
// }

// void CScript::initialize()
// {
//   // HA_LOG_NFO(m_file.string() << " (" << m_name << ") content:\n---\n" << content() << "\n---");
//   // CScriptBuilder builder;
//   // builder.StartNewModule()

//   m_context = HA_ST.angel().engine()->asEngine()->CreateContext();
//   if(!m_context) { callDestroySelf("Can not create context"); return; }
//   m_module = HA_ST.angel().engine()->asEngine()->GetModule(m_name.c_str(), asGM_ALWAYS_CREATE);
//   m_module->AddScriptSection("script", content().c_str());
//   if(m_module->Build() < 0) { callDestroySelf("Can not build"); return; }

//   callMethod("void initialize()");
// }

// void CScript::callMethod(const std::string &method)
// {
//   asIScriptFunction *function = m_module->GetFunctionByDecl(method.c_str());
//   if(function)
//   {
//     m_context->Prepare(function);
//     if(m_context->Execute() != asEXECUTION_FINISHED) { callDestroySelf(method + " failed"); }
//     m_context->Release();
//   }
// }

// std::string CScript::content()
// {
//   std::ifstream ifs(m_file.string());
//   return std::string ( (std::istreambuf_iterator<char>(ifs) ),
//                        (std::istreambuf_iterator<char>()    ) );
// }

// void CScript::callDestroySelf(const std::string &message)
// {
//   HA_LOG_ERR("[" << m_name << "] " << message);
//   HA_ST.angel().engine()->detachScript(m_file);
// }



// bool CScript::operator==(const CScript &other) const
// {
//   return m_file == other.file();
// }
}
}
