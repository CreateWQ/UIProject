#include "file_manager.hpp"
#include "GLFW/glfw3.h"
#include <stdexcept>

FileManager::FileManager(std::shared_ptr<LoggerFacade> log, std::shared_ptr<ImageProcessor> image_proc)
    : current_path(std::filesystem::current_path()), logger(log), image_processor(image_proc) {
    refresh();
    if (logger) logger->info("FileManager initialized at path: {}", current_path.string());
}

FileManager::~FileManager() {

    for (auto& [path, texture] : thumbnail_cache) {
        if (texture) glDeleteTextures(1, &texture);
    }
    if (logger) logger->info("File Manager destroyed");
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
        if (thumbnail_cache.count(path.string())) {
            glDeleteTextures(1, &thumbnail_cache[path.string()]);
            thumbnail_cache.erase(path.string());
        }

        refresh();
        if (logger) logger->info("Deleted file: {}", path.string());
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to delete file: {}", e.what());
    }

}

unsigned int FileManager::getThumbnail(const std::string& path) {
    if (thumbnail_cache.count(path)) return thumbnail_cache[path];

    cv::Mat thumbnail = image_processor->generateThumbnail(path);
    if (thumbnail.empty()) return 0;

    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, thumbnail.cols, thumbnail.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, thumbnail.data);
    glBindTexture(GL_TEXTURE_2D, 0);

    thumbnail_cache[path] = texture;
    return texture;
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

