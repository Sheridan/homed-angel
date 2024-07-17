#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <sys/inotify.h>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <limits.h>


namespace ha
{
namespace scripting
{
namespace fs
{

class CFileWatcher {
public:
    explicit CFileWatcher(const std::filesystem::path &directory);
            ~CFileWatcher();

    void watch();
    void stop();

private:
    void addWatch(const std::filesystem::path& path);
    void removeWatch(int wd);
    void processEvent(const std::string& directory, const struct inotify_event* event);

    void initializeWatchers();
    void scanDirectory(const std::filesystem::path& path);

    bool containsInitializeFunction(const std::filesystem::path& filepath);

    std::filesystem::path m_directory;
    int m_inotify_fd;
    std::unordered_map<int, std::string> m_wd_to_directory;
    bool m_running;
};

}
}
}
