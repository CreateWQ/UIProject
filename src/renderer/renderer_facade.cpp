#include "renderer_facade.hpp"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_freetype.h>
#include <stdexcept>

void RendererFacade::initGLFW() {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "ImGui MVC Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void RendererFacade::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImFont* front_regular = io.Fonts->AddFontFromFileTTF("fonts/WinkyRough-Italic-VariableFont_wght.ttf", 25.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
    
    static ImFontConfig config;
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("fonts/KosugiMaru-Regular.ttf", 17.5f, &config, io.Fonts->GetGlyphRangesChineseFull());
    // config.PixelSnapH = true;
    config.OversampleH = config.OversampleV = 1;
    
    static const ImWchar emoji_ranges[] = {
        0x1, 0xEFFFFFF,
        0,               // Range terminator
    };

    config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    io.Fonts->AddFontFromFileTTF("fonts/NotoColorEmoji-Regular.ttf", 25.0f, &config, emoji_ranges);
    
    io.Fonts->Build();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

RendererFacade::RendererFacade() 
    : window(nullptr), initialized(false) {
    initGLFW();
    initImGui();
    initialized = true;
}

RendererFacade::~RendererFacade() {
    if (initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
}

GLFWwindow* RendererFacade::getWindow() const { return window; }

void RendererFacade::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RendererFacade::endFrame() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.45f, 0.45f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}