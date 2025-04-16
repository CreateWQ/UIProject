#define _CRT_SECURE_NO_WARNINGS

#include "theme_manager.hpp"

#include <imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <fstream>
#include <stdexcept>

ThemeManager::ThemeManager(std::shared_ptr<LoggerFacade> log, const std::string& path)
    : current_theme(Theme::Dark), logger(log), config_path(path) {

    try {
        FILE* fp = fopen(config_path.c_str(), "rb");
        if (fp) {
            char readBuffer[65536];
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
            rapidjson::Document doc;
            doc.ParseStream(is);
            fclose(fp);
            if (doc.IsObject() && doc.HasMember("theme")) {
                std::string theme_str = doc["theme"].GetString();
                if (theme_str == "Light") current_theme = Theme::Light;
                else if (theme_str == "Classic") current_theme = Theme::Classic;
            }
        }
        applyTheme(current_theme);
        if (logger) 
            logger->info("ThemeManager initialized with theme: {}", 
                current_theme == Theme::Dark  ? "Dark" : 
                current_theme == Theme::Light ? "Light" : 
                                                "Classic"
            );
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to load theme {}", e.what());
    }

}

void ThemeManager::applyTheme(Theme theme) {
    current_theme = theme;
    switch (current_theme) {
        case Theme::Dark: 
            ImGui::StyleColorsDark();
            break;
        case Theme::Light: 
            ImGui::StyleColorsLight();
            break;
        case Theme::Classic: 
            ImGui::StyleColorsClassic();
            break;
    }
    if (logger) 
        logger->info("Applied theme: {}", 
            current_theme == Theme::Dark  ? "Dark" : 
            current_theme == Theme::Light ? "Light" : 
                                        "Classic"
        );
    saveTheme();
}

void ThemeManager::saveTheme() {

    try {
        // 
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
        std::string theme_str = current_theme == Theme::Dark  ? "Dark" : 
                                current_theme == Theme::Light ? "Light" : 
                                                                "Classic";  
        doc.AddMember("theme", rapidjson::Value(theme_str.c_str(), allocator), allocator); // 將 theme 添加進 allocator
        
        // 設置緩存器，將 theme 寫入 json 中
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        FILE* fp = fopen(config_path.c_str(), "wb");
        if (fp) {
            const size_t size = buffer.GetSize();
            char* str = (char *)malloc(sizeof(char) * size);
            std::memcpy(str, buffer.GetString(), size); 
            rapidjson::FileWriteStream os(fp, str, buffer.GetSize());
            rapidjson::Writer<rapidjson::FileWriteStream> file_writer(os);
            doc.Accept(file_writer);
            fclose(fp);

            if (logger) logger->debug("Theme saved to {}", config_path);
        }

    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to save theme: {}", e.what());
    }

}