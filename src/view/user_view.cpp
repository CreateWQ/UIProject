#include "user_view.hpp"
#include <imgui.h>

UserView::UserView(
    std::shared_ptr<UserModel> m, 
    std::shared_ptr<LoggerFacade> log
) : model(m), 
    logger(log), 
    inputBuffer(m->getUsername()) 
{
    if (logger) logger->info("UserView initialized");
} 

void UserView::update() {
    inputBuffer = model->getUsername();
    if (logger) logger->debug("UserView updated with new username: {}", inputBuffer);
}

void UserView::render() {
    ImGui::Begin("User Profile");
    ImGui::Text("Username: %s", model->getUsername().c_str());
    ImGui::InputText("New Username", &inputBuffer[0], inputBuffer.size() + 1);
    if (ImGui::Button("Update")) {
        model->setUsername(inputBuffer);
        if (logger) logger->info("User clicked Updated button");
    }
    ImGui::End();
}