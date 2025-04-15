#pragma once 

struct GLFWwindow;

class RendererFacade {
private:

    GLFWwindow* window;
    bool initialized;

    void initGLFW();

    void initImGui();

public:

    RendererFacade();

    ~RendererFacade();

    GLFWwindow* getWindow() const;

    void beginFrame();

    void endFrame();

};