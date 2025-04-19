
#include "application.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Application::Application() {
    logger = std::make_shared<LoggerFacade>();
    logger->info("Application starting");

    renderer = std::make_unique<RendererFacade>(logger);
    
    theme_manager = std::make_shared<ThemeManager>(logger);
    image_processor = std::make_shared<ImageProcessor>(logger);
    file_manager = std::make_shared<FileManager>(logger, image_processor);
    model = std::make_shared<UserModel>("Alice", logger);
    view = UIFactory::createUserView(model, file_manager, theme_manager, image_processor, logger);
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