cmake_minimum_required(VERSION 3.10.0)
set(PROJECT_NAME ui_project)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_FILE_DIR ${CMAKE_SOURCE_DIR}/src/)

file(GLOB src_file ${SOURCE_FILE_DIR}/*.cpp)
file(GLOB imgui_src_file ${SOURCE_FILE_DIR}/*/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${src_file} ${imgui_src_file})

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)