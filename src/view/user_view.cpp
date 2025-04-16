#include "user_view.hpp"
#include <iostream>
#include <imgui.h>

#pragma execution_character_set("utf-8")

UserView::UserView(
    std::shared_ptr<UserModel> m,
    std::shared_ptr<FileManager> file,
    std::shared_ptr<ThemeManager> theme,
    std::shared_ptr<LoggerFacade> log
) : model(m), 
    logger(log), 
    file_manager(file),
    theme_manager(theme),
    inputBuffer(m->getUsername()),
    file_filter_buffer(".png"),
    selected_theme(static_cast<int>(theme_manager->getCurrentTheme())) 
{
    file_manager->setFilter(file_filter_buffer);
    if (logger) logger->info("UserView initialized");
} 

void UserView::update() {
    inputBuffer = model->getUsername();
    if (logger) logger->debug("UserView updated with new username: {}", inputBuffer);
}

void UserView::render() {

    ImGui::Begin("Settings");
    const char* themes[] = {"Dark", "Light", "Classic"};
    if (ImGui::Combo("Theme", &selected_theme, themes, IM_ARRAYSIZE(themes))) {
        theme_manager->applyTheme(static_cast<ThemeManager::Theme>(selected_theme));
    }
    ImGui::End();

    ImGui::Begin("User Profile");
    ImGui::Text("Username: %s", model->getUsername().c_str());
    ImGui::InputText("New Username", &inputBuffer[0], inputBuffer.size() + 1);
    if (ImGui::Button("Update")) {
        model->setUsername(inputBuffer);
        if (logger) logger->info("User clicked Updated button");
    }
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
    // image proccessor;

    // -----------------
    ImGui::BeginChild("FileScroll", ImVec2(0, 0), true);
    for (const auto& entry : file_manager->getEntries()) {
        std::string name = entry.path().filename().string();
        if (entry.is_directory()) {
            if (ImGui::Selectable(("📁 " + name).c_str())) {
                file_manager->changeDirectory(entry.path());
            }
        } else {
            if (ImGui::Selectable(("📄 " + name).c_str())) {
                if (logger) logger->info("Selectable file: {}", name);
            }
            if (ImGui::BeginPopupContextItem(name.c_str())) {
                if (file_manager->isImageFile(entry.path()) && ImGui::MenuItem("Load Image")) {
                    // load image
                }
                if (ImGui::MenuItem("Delete")) {
                    file_manager->deleteFile(entry.path());
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::EndChild();
    ImGui::End();

}