#pragma once 

#include "logger/logger_facade.hpp"
#include <filesystem>
#include <memory>
#include <vector>

class FileManager {
private:

    std::filesystem::path current_path;
    std::vector<std::filesystem::directory_entry> entries;
    std::shared_ptr<LoggerFacade> logger;
    std::string filter;

public:

    FileManager(std::shared_ptr<LoggerFacade> log);

    void refresh();
    
    void changeDirectory(const std::filesystem::path& path);
    void deleteFile(const std::filesystem::path& path);
    void setFilter(const std::string& e);   
    bool isImageFile(const std::filesystem::path& path) const;
    bool existPath(const std::filesystem::path& path) const; 
    void createDirectory(const std::filesystem::path& path) const;

    inline const std::filesystem::path getCurrentPath() const { return current_path; }
    inline const std::vector<std::filesystem::directory_entry>& getEntries() const { return entries; }
};