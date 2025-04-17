cmake_minimum_required(VERSION 3.11.0)
set(PROJECT_NAME ui_project)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS on)

set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")
# set(CMAKE_PREFIX_PATH "C:/src/vcpkg/installed/x64-windows/")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_FILE_DIR ${CMAKE_SOURCE_DIR}/src/)

file(GLOB src_file ${SOURCE_FILE_DIR}/*.cpp)
file(GLOB imgui_src_file ${SOURCE_FILE_DIR}/*/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(RapidJSON_DIR ${CMAKE_SOURCE_DIR}/include/)

add_executable(${PROJECT_NAME} ${src_file} ${imgui_src_file})

set(freetype_DIR "C:/src/vcpkg/packages/freetype_x64-windows/share/freetype")
set(FREETYPE_LIBRARY "C:/src/vcpkg/packages/freetype_x64-windows/lib")
set(FREETYPE_INCLUDE_DIRS "C:/src/vcpkg/packages/freetype_x64-windows/include")

set(ZLIB_DIR "C:/src/vcpkg/packages/zlib_x64-windows/share/zlib")
set(ZLIB_LIBRARY "C:/src/vcpkg/packages/zlib_x64-windows/lib")
set(ZLIB_INCLUDE_DIR "C:/src/vcpkg/packages/zlib_x64-windows/include")

# set(lunasvg_DIR "C:/src/vcpkg/packages/lunasvg_x64-windows/share/lunasvg")
set(plutovg_DIR "C:/src/vcpkg/packages/plutovg_x64-windows/share/plutovg")
set(plutosvg_DIR "C:/src/vcpkg/packages/plutosvg_x64-windows/share/plutosvg")

set(OpenCV_DIR "C:/opencv/opencv/build")

find_package(OpenCV REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include_directories(${RapidJSON_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC 
    imgui::imgui 
    glfw
    spdlog::spdlog_header_only
    ${OpenCV_LIBS}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/src/vcpkg/packages/freetype_x64-windows/bin/freetype.dll"
            "${CMAKE_SOURCE_DIR}/bin/Debug/freetyped.dll"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/opencv/opencv/build/x64/vc16/bin/opencv_world4100d.dll"
            "${CMAKE_SOURCE_DIR}/bin/Debug/opencv_world4100d.dll"
)