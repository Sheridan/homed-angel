#include "scripting/helpers/clogger.h"


#include "clogger.h"
#include "log.h"
namespace ha
{
namespace scripting
{
namespace helpers
{

CLogger::CLogger(const std::string &name)
  : m_name(name)
{
}

CLogger::~CLogger()
{
}

void CLogger::log(const ELogPriority &pri, const std::string &message)
{
  *(static_cast<std::ostream *>(m_outs[pri])) << "[" << m_name << ":" << m_priorities[pri] << "] " << message << std::endl;
}

}
}
}
