#pragma once
#include <vector>
#include "scripting/script/cscript.h"

namespace ha
{
namespace scripting
{

class CScriptsManager
{
public:
  explicit CScriptsManager();
  virtual ~CScriptsManager();

  void attachScript(const std::filesystem::path &file);
  void detachScript(const std::filesystem::path &file);

  CScript *script(const std::string &name);

private:
  std::vector<CScript *> m_scripts;
  CScript *script(const std::filesystem::path &file);
};

}
}
