# UIProject

**Language: [English](README.md) | [Chinese](README.zh.md)**

---

A C++ application using ImGui, GLFW, and spdlog to implement a UI interface with MVC, Application, and Logger patterns.

## Folder Structure

- **src/**: Source code, organized by module(application, model, view, renderer, logger, factory).
- **logs/**: Log output directory for spdlog.

## Dependencies

- [GLFW](https://www.glfw.org/)
- [OpenGL](https://www.opengl.org/)
- [spdlog](https://github.com/gabime/spdlog)
- [ImGui](https://github.com/ocornut/imgui)
- [vcpkg](https://github.com/microsoft/vcpkg) (dependency manager).

## Prerequisites

Install vpckg to manage dependencies:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vpckg
./bootstrap-vcpkg.sh  # Linux/macOS
.\bootstrap-vcpkg.bat # Windows
```

If you don't know whether the dependencies you want to use is managed by vpckg, search dependencies via vcpkg:

```bash
vcpkg search glfw3 # you can change "glfw3" to other dependencies
```

Install vpckg to manage dependencies:

```bash
vcpkg install glfw3 
vpckg install opengl
vcpkg install spdlog
vpckg install imgui[docking-experimental,opengl3-binding,glfw-binding]
```

## Build Instructions

### Windows

1. Install [CMake](https://cmake.org/download/) and a C++ compiler (e.g., [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/), [Clang](https://clang.llvm.org/))

2. Configure and build:

    ```bash
    mkdir build
    cd build 
    cmake .. -DCMAKE_TOOLCHAIN_FILE="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    ```

3. Run:

    ```bash
    cd ..
    ./bin/Debug/ui_project
    ```

### Linux

1. Install CMake and a C++ compiler (e.g., g++).

   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```

2. Configure and build:

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

3. Run:

   ```bash
   cd ..
   ./bin/ui_project
   ```

### macOS

1. Install CMake and a C++ compiler (e.g., clang via Xcode).

   ```bash
   brew install cmake
   ```

2. Configure and build:

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

3. Run:

   ```bash
   cd .. 
   ./bin/ui_project
   ```

## Features

- MVC pattern for UI (UserModel, UserView).
- Application pattern for lifecycle management.
- LoggerFacade with spdlog for console and file logging.
- Smart pointers for memory safety.

## Log Output

Logs are written to `logs/app.log` and displayed in the console.
