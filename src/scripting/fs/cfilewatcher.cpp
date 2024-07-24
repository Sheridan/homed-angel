#include "scripting/fs/cfilewatcher.h"
#include <regex>
#include <thread>
#include <iostream>
#include <fstream>
#include "st.h"

namespace ha
{
namespace scripting
{
namespace fs
{

CFileWatcher:: CFileWatcher(const std::filesystem::path &directory)  : m_directory(directory), m_inotify_fd(-1), m_running(false)
{
  if (!std::filesystem::exists      (m_directory)) { HA_ABORT(m_directory.string() << " is not exists"     ); }
  if (!std::filesystem::is_directory(m_directory)) { HA_ABORT(m_directory.string() << " is not a directory"); }

  initializeWatchers();
}

CFileWatcher::~CFileWatcher()
{
  if (m_inotify_fd != -1)
  {
    for (const auto& [wd, _] : m_wd_to_directory)
    {
      inotify_rm_watch(m_inotify_fd, wd);
    }
    close(m_inotify_fd);
  }
}

void CFileWatcher::stop()
{
  m_running = false;
}

void CFileWatcher::initializeWatchers()
{
  m_inotify_fd = inotify_init1(IN_NONBLOCK);
  if (m_inotify_fd == -1)
  {
    perror("inotify_init1");
    HA_ABORT("Failed to inotify_init1");
  }

  try
  {
    addWatch(m_directory);
    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_directory))
    {
      if (entry.is_directory())
      {
        addWatch(entry.path());
      }
    }
  }
  catch (const std::exception& e)
  {
    HA_ABORT("Failed to initialize watches: " << e.what());
  }
}

void CFileWatcher::scanDirectory(const std::filesystem::path& path)
{
  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    if (std::filesystem::is_directory(entry.path()))
    {
      scanDirectory(entry.path()); // Рекурсивный вызов для подкаталогов
    }
    else if (entry.path().extension() == ".as")
    {
      if(containsInitializeFunction(entry))
      {
        HA_ST->angel()->manager()->attachScript(entry);
      }
    }
  }
}

bool CFileWatcher::containsInitializeFunction(const std::filesystem::path &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "Unable to open file: " << filepath << std::endl;
    return false;
  }

  std::string line;
  std::regex re(R"(\s*void\s+initialize\s*\(\s*\)\s*\{?)");

  while (std::getline(file, line))
  {
    if (std::regex_search(line, re))
    {
      return true;
    }
  }
  return false;
}

void CFileWatcher::watch()
{
  HA_LOG_NFO("Using scripts directory: " << m_directory);

  scanDirectory(m_directory);

  const size_t event_size = sizeof(struct inotify_event);
  const size_t buf_len = 1024 * (event_size + NAME_MAX + 1);
  char buffer[buf_len];

  m_running = true;
  while (m_running)
  {
    HA_ST->sleep();

    int length = read(m_inotify_fd, buffer, buf_len);
    if (length < 0 && errno != EAGAIN)
    {
      perror("read");
      break;
    }

    if (length <= 0) continue;

    for (char* ptr = buffer; ptr < buffer + length; ptr += event_size + ((struct inotify_event *) ptr)->len)
    {
      struct inotify_event* event = (struct inotify_event *) ptr;
      processEvent(m_wd_to_directory.at(event->wd), event);
    }
  }
}

void CFileWatcher::addWatch(const std::filesystem::path &path)
{
  int wd = inotify_add_watch(m_inotify_fd, path.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE | IN_ONLYDIR);
  if (wd == -1)
  {
    perror("inotify_add_watch");
    throw std::runtime_error("Failed to add inotify watch");
  }
  m_wd_to_directory[wd] = path.string();
}

void CFileWatcher::removeWatch(int wd)
{
  if (inotify_rm_watch(m_inotify_fd, wd) == -1)
  {
    perror("inotify_rm_watch");
  }
  m_wd_to_directory.erase(wd);
}

void CFileWatcher::processEvent(const std::string &directory, const inotify_event *event)
{
  std::string file_path = directory + "/" + event->name;

  if (event->mask & (IN_CREATE | IN_MODIFY))
  {
    if (std::filesystem::is_regular_file(file_path) && file_path.ends_with(".as"))
    {
      std::filesystem::path file = file_path;
      HA_LOG_NFO("Script file changed: " << file.parent_path().string() << "/" << file.stem().string() << file.extension().string());
      if(containsInitializeFunction(file))
      {
        HA_ST->angel()->manager()->attachScript(file);
      }
    }

    if (std::filesystem::is_directory(file_path))
    {
      addWatch(file_path);
    }
  }

  if (event->mask & IN_DELETE && std::filesystem::is_directory(file_path))
  {
    for (const auto& [wd, dir] : m_wd_to_directory)
    {
      if (dir == file_path)
      {
        removeWatch(wd);
        break;
      }
    }
  }
}


}
}
}
