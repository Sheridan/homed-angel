#include "scripting/cengine.h"
#include <string>
#include <cassert>
#include <iostream>
#include <memory>
#include "scripting/helpers/clogger.h"
#include "scripting/helpers/cstrings.h"
#include "st.h"



namespace ha
{
namespace scripting
{

// CEngine::CEngine()
//   // : m_asEngine(nullptr),
//   //   m_logger(nullptr)
// {
//   // m_asEngine = asCreateScriptEngine();
//   // assert(m_asEngine != nullptr && "Failed to create AngelScript m_asEngine");
//   // registerEntities();
// }

// CEngine::~CEngine()
// {
//   // if (m_asEngine)
//   // {
//   //   m_asEngine->ShutDownAndRelease();
//   // }
//   // delete m_logger;
// }

// CScript *CEngine::script(const std::filesystem::path &file)
// {
//   for(CScript *script : m_scripts)
//   {
//     if(script->file() == file) { return script; }
//   }
//   return nullptr;
// }

// void CEngine::attachScript(const std::filesystem::path &file)
// {
//   if (!std::filesystem::exists(file)) { HA_ABORT(file.string() << " is not exists o0"); }
//   detachScript(file);
//   m_scripts.push_back(new CScript(file));
// }

// void CEngine::detachScript(const std::filesystem::path &file)
// {
//   for (auto it = m_scripts.begin(); it != m_scripts.end();)
//   {
//     CScript *script = *it;
//     if(script->file() == file)
//     {
//       delete script;
//       it = m_scripts.erase(it);
//     }
//     else
//     {
//       ++it;
//     }
//   }
// }

// void CEngine::messageCallback(const asSMessageInfo *msg)
// {
//                                      const char* type = "err";
//        if (msg->type == asMSGTYPE_WARNING)     { type = "wrn"; }
//   else if (msg->type == asMSGTYPE_INFORMATION) { type = "inf"; }
//   HA_LOG_SCRIPT(type, msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message);
// }



}
}
