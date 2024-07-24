#include "scripting/cangel.h"
#include "st.h"

namespace ha
{
namespace scripting
{

CAngel::CAngel()
{
  m_manager = new CScriptsManager();
  m_fileWatcher = new ha::scripting::fs::CFileWatcher(HA_ST->config()->scriptingLocation());
}

CAngel::~CAngel()
{
  delete m_fileWatcher;
  delete m_manager;
}

void CAngel::start()
{
  m_fileWatcher->watch();
}

void CAngel::stop()
{
  m_fileWatcher->stop();
}

}
}
