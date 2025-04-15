cmake_minimum_required(VERSION 3.10.0)
project(ImGUI_Project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB src_file src/*.cpp)


add_executable(ImGUI_Project ${src_file})

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(ImGUI_Project PUBLIC imgui::imgui)
target_link_libraries(ImGUI_Project PUBLIC glfw)
target_link_libraries(ImGUI_Project PUBLIC spdlog::spdlog_header_only)