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
