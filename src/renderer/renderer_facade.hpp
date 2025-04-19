#pragma once 

#include <logger/logger_facade.hpp>

struct GLFWwindow;

class RendererFacade {
private:

    GLFWwindow* window;
    bool initialized;

    std::shared_ptr<LoggerFacade> logger;

    inline void initGLFW();

    inline void initImGui();

public:

    RendererFacade(std::shared_ptr<LoggerFacade> log);

    ~RendererFacade();

    GLFWwindow* getWindow() const;

    void beginFrame();

    void endFrame();

};