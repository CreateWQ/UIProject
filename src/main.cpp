#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "Application/application.hpp"

/* class Application {
private:

    std::unique_ptr<RendererFacade> renderer;
    std::shared_ptr<LoggerFacade> logger;
    std::shared_ptr<UserModel> model;
    std::unique_ptr<UserView> view;

public:

    Application() {
        logger = std::make_shared<LoggerFacade>();
        logger->info("Application starting");

        try {
            renderer = std::make_unique<RendererFacade>();
            logger->info("Renderer initialized");
        } catch (const std::exception& e) {
            logger->error("Renderer initialization failed: {}", e.what());
            throw;
        }

        model = std::make_shared<UserModel>("Alice", logger);
        view = UIFactory::createUserView(model, logger);
    }

    void run() {
        logger->info("Application running");
        GLFWwindow* window = renderer->getWindow();

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            renderer->beginFrame();
            view->render();
            renderer->endFrame();
        }
        logger->info("Application shutting down");
    }

}; */

int main(int, char**){
    
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {

        fprintf(stderr, "Error: %s\n", e.what());
        return -1;

    }
    return 0;
}
