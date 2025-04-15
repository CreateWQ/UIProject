#include "application.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Application::Application() {
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

void Application::run() {
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