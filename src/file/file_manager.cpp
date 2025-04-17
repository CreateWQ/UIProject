#include "file_manager.hpp"
#include <stdexcept>

FileManager::FileManager(std::shared_ptr<LoggerFacade> log)
    : current_path(std::filesystem::current_path()), logger(log) {
    refresh();
    if (logger) logger->info("FileManager initialized at path: {}", current_path.string());
}

void FileManager::refresh() {

    entries.clear();
    try {
        for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
            if (filter.empty() || entry.path().extension() == filter || entry.is_directory()) {
                entries.push_back(entry);
            }
        }
        if (logger) logger->debug("Refreshed directory: {} entries found", entries.size());
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to refresh directory: {}", e.what());
    }

}

void FileManager::changeDirectory(const std::filesystem::path& path) {

    try {
        current_path = path;
        refresh();
        if (logger) logger->info("Changed directory to: {}", current_path.string());
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to change directory to: {}", e.what());
    }

}

void FileManager::deleteFile(const std::filesystem::path& path) {
    
    try {
        std::filesystem::remove(path);
        refresh();
        if (logger) logger->info("Deleted file: {}", path.string());
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to delete file: {}", e.what());
    }

}

void FileManager::setFilter(const std::string& ext) {

    filter = ext;
    refresh();
    if (logger) logger->debug("Set file filter to: {}", filter.empty() ? "None" : filter);

}

bool FileManager::isImageFile(const std::filesystem::path& path) const {

    std::string ext = path.extension().string();

    return ext == ".png" || ext == ".jpg" || ext == ".jpeg";

}

bool FileManager::existPath(const std::filesystem::path& path) const {

    return std::filesystem::exists(path);

}

void FileManager::createDirectory(const std::filesystem::path& path) const {

    try {
        std::filesystem::create_directory(path);
        if (logger) logger->info("Create directory: {}", path.string());
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to create directory: {}", e.what());
    }

}

