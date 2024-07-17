#pragma once
#include "scripting/fs/cfilewatcher.h"
#include "scripting/script/cscriptsmanager.h"

namespace ha
{
namespace scripting
{

class CAngel
{
public:
  CAngel();
  ~CAngel();

  void start();
  void stop();

  CScriptsManager *manager() {return m_manager; };

private:
  CScriptsManager *m_manager;
  ha::scripting::fs::CFileWatcher *m_fileWatcher;

};

}
}
