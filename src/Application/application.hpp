#pragma once

#include "renderer/renderer_facade.hpp"
#include "model/user_model.hpp"
#include "view/user_view.hpp"
#include "logger/logger_facade.hpp"
#include "factory/ui_factory.hpp"
#include <memory>

class Application {
private:

    std::unique_ptr<RendererFacade> renderer;
    std::shared_ptr<LoggerFacade> logger;
    std::shared_ptr<FileManager> file_manager;
    std::shared_ptr<ThemeManager> theme_manager;
    std::shared_ptr<UserModel> model;
    std::unique_ptr<UserView> view;

public:

    Application();

    void run();

};
