#include "user_view.hpp"
#include <iostream>
#include <imgui.h>

// #pragma execution_character_set("utf-8")

static std::string file_path;

UserView::UserView(
    std::shared_ptr<UserModel> m,
    std::shared_ptr<FileManager> file,
    std::shared_ptr<ThemeManager> theme,
    std::shared_ptr<ImageProcessor> image,
    std::shared_ptr<LoggerFacade> log
) : model(m), 
    logger(log), 
    file_manager(file),
    theme_manager(theme),
    image_processor(image),
    inputBuffer(m->getUsername()),
    selected_log_level(0),
    selected_strategy(0),
    file_filter_buffer(".png"),
    execute_path(file_manager->getCurrentPath().string()),
    selected_theme(static_cast<int>(theme_manager->getCurrentTheme())) 
{
    
    processing_params[0] = 5.0f;
    processing_params[1] = 100.0f; // Canny low threshold
    processing_params[2] = 200.0f; // Canny high threshold
    
    file_manager->setFilter(file_filter_buffer);
    if (logger) logger->info("UserView initialized");
} 

void UserView::update() {
    inputBuffer = model->getUsername();
    if (logger) logger->debug("UserView updated with new username: {}", inputBuffer);
}

void UserView::render() {

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

    ImGui::Begin("Settings");
    const char* themes[] = {"Dark", "Light", "Classic"};
    if (ImGui::Combo("Theme", &selected_theme, themes, IM_ARRAYSIZE(themes))) {
        theme_manager->applyTheme(static_cast<ThemeManager::Theme>(selected_theme));
    }
    ImGui::End();

    // User information
    ImGui::Begin("User Profile");
    ImGui::Text("Username: %s", model->getUsername().c_str());
    ImGui::InputText("New Username", &inputBuffer[0], inputBuffer.size() + 1);
    if (ImGui::Button("Update")) {
        model->setUsername(inputBuffer);
        if (logger) logger->info("User clicked Updated button");
    }
    ImGui::End();

    ImGui::Begin("Log Window");
    const char* log_levels[] = {"All", "Info", "Debug", "Error"};
    ImGui::Combo("Filter", &selected_log_level, log_levels, IM_ARRAYSIZE(log_levels));
    ImGui::BeginChild("LogScroll", ImVec2(0, 0), true);
    
    if (ImGui::Button("Clear Logs")) {
        logger->clearLogBuffers();
    }    

    for (const auto& [level, str] : logger->getLogBuffers()) {
        if (selected_log_level == 0 || 
            selected_log_level == 1 && level == spdlog::level::info ||
            selected_log_level == 2 && level == spdlog::level::debug ||
            selected_log_level == 3 && level == spdlog::level::err
        ) {
            ImGui::TextUnformatted(str.c_str());
        }
    }
    ImGui::EndChild();
    ImGui::End();

    // 檔案管理器
    ImGui::Begin("File Manager");
    ImGui::Text("Current Path: %s", file_manager->getCurrentPath().string().c_str());
    ImGui::InputText("Filter", &file_filter_buffer[0], file_filter_buffer.size() + 1);
    if (ImGui::Button("Apply Filter")) {
        file_manager->setFilter(file_filter_buffer);
    }
    ImGui::SameLine();
    if (ImGui::Button("Parent Directory") && file_manager->getCurrentPath().has_parent_path()) {
        file_manager->changeDirectory(file_manager->getCurrentPath().parent_path());
    }
    
    if (image_processor->hasImageLoaded() && ImGui::Button("Save Processed")) {
        std::string save_path = (execute_path + "/output/");
        
        if (!file_manager->existPath(save_path)) {
            file_manager->createDirectory(save_path);
        }

        image_processor->saveProcessed(save_path + "processed_image.png");
    }   
    // 拖放檔案
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
            const char* path = (const char*)payload->Data;
            if (logger) logger->debug("{}", path);
            if (file_manager->isImageFile(path)) {
                image_processor->loadImage(path);
                if (logger) logger->info("Drop image: {}", path);
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (image_processor->hasImageLoaded()) {
        ImGui::Begin("Image Editor");
        const char* strategies[] = {"Grayscale", "Blur", "Canny"};
        if (ImGui::Combo("Processing Strategy", &selected_strategy, strategies, IM_ARRAYSIZE(strategies))) {
            std::vector<float> params(
                processing_params + (selected_strategy == 2 ? 1 : 0), 
                processing_params + (selected_strategy == 2 ? 3 : 1)
            );
            image_processor->processImage(selected_strategy, params);
        }
        if (selected_strategy == 1) {
            ImGui::SliderFloat("Kernel Size", &processing_params[0], 3.0f, 15.0f, "%.0f");
        } else if (selected_strategy == 2) {
            ImGui::SliderFloat("Low Threshold", &processing_params[1], 0.0f, 300.0f, "%.0f");
            ImGui::SliderFloat("High Threshold", &processing_params[2], 0.0f, 300.0f, "%.0f");
        }

        if (ImGui::Button("Apply")) {
            std::vector<float> params(
                processing_params + (selected_strategy == 2 ? 1 : 0), 
                processing_params + (selected_strategy == 2 ? 3 : 1)
            );
            image_processor->processImage(selected_strategy, params);
        }
        ImGui::End();
    }

    ImGui::BeginChild("FileScroll", ImVec2(0, 0), true);
    for (const auto& entry : file_manager->getEntries()) {
        std::string name = entry.path().filename().string();
        std::string display_name = entry.is_directory() ? "📁 " + name : "📄 " + name;
        
        if (ImGui::Selectable(display_name.c_str())) {

            if (entry.is_directory()) file_manager->changeDirectory(entry.path());

            if (logger) logger->info("Selected file: {}", name);

        }
        
        if (!entry.is_directory() && ImGui::BeginPopupContextItem(name.c_str())) {
            if (file_manager->isImageFile(entry.path()) && ImGui::MenuItem("Load Image")) {
                image_processor->loadImage(entry.path().string());
            }
            if (ImGui::MenuItem("Delete")) {
                file_manager->deleteFile(entry.path());
            }
            ImGui::EndPopup();
        }

    }
    // if (logger) logger->debug("{}", image_processor->hasImageLoaded());
    ImGui::EndChild();
    ImGui::End();
    
    if (image_processor->hasImageLoaded()) {
        ImGui::Begin("Image Viewer");
        auto size = image_processor->getImageSize();
        ImGui::Text("Original Image");
        ImGui::SameLine();
        ImGui::Text("Grayscale Image");
        ImGui::Image((intptr_t)image_processor->getOriginalTexture(), ImVec2((float)size.width, (float)size.height));
        ImGui::SameLine();
        ImGui::Image((intptr_t)image_processor->getProcessedTexture(), ImVec2((float)size.width, (float)size.height));
        ImGui::End();
    }

    /* static bool show_window = true;
    
    if (show_window) {
        if (ImGui::Begin("Test Window", &show_window)) {
            ImGui::Text("bye bye");
        }
        ImGui::End();
    
    } */

    ImGui::Begin("Drag and Drop Test");

    ImGui::Text("將圖片文件拖放到此窗口");
        ImGui::Separator();
        
        // 拖放目標區域
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_IMAGE"))
            {
                // 從 payload 獲取文件路徑
                const char* path = (const char*)payload->Data;
                file_path = path;
                
                // 嘗試載入圖片
                image_processor->loadImage(file_path);
                if (logger) logger->debug("Load file from {}", file_path);
            }
            ImGui::EndDragDropTarget();
        }

    ImGui::End();

}