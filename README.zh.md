# UI 專案

**語言: [英文](README.md) | [中文](README.zh.md)**

## 概述

這是一個使用 ImGui、GLFW 和 spdlog 實現的 C++ 應用程式，採用 MVC、Application 和 Logger 設計模式來構建 UI 介面。

## 資料夾結構

- **src/**：原始碼，按模組組織（application、model、view、renderer、logger、factory）。
- **logs/**：spdlog 的日誌輸出目錄。

## 依賴

- [GLFW](https://www.glfw.org/)
- [OpenGL](https://www.opengl.org/)
- [spdlog](https://github.com/gabime/spdlog)
- [ImGui](https://github.com/ocornut/imgui)（以原始碼形式包含在 `include/imgui` 中）
- [RapidJSON](https://github.com/Tencent/rapidjson) (`inlcude/rapidjson`)
- [Noto Color Emoji](https://fonts.google.com/noto/specimen/Noto+Color+Emoji)(Emoji 圖標📁、📄)
- [Winky Rough](https://fonts.google.com/specimen/Winky+Rough) (英文字型)
- [Kosugi Maru](https://fonts.google.com/specimen/Kosugi+Maru) (中文字型)
- [vcpkg](https://github.com/microsoft/vcpkg)（依賴管理工具）

### 前置條件

安裝 vcpkg 以管理依賴：

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
.\bootstrap-vcpkg.bat # Windows
```

如果你不知道你要用的 Dependencies 是否有被 vcpkg 管理，可以透過 vpckg 搜索 Dependencies：

```bash
vcpkg search glfw3 # 你可以把 "glfw3" 換成其他的 Dependencies 
```

透過 vcpkg 安裝 Dependencies：

```bash
vcpkg install glfw3 
vpckg install opengl
vcpkg install spdlog
vpckg install imgui[docking-experimental,opengl3-binding,glfw-binding]
```

## 建構說明

### Windows

1. 安裝 [CMake](https://cmake.org/download/) 和 C++ 編譯器（例如 [Visual Studio 2022](https://visualstudio.microsoft.com/download/)、[Clang](https://clang.llvm.org/)）。
2. 配置並建構：

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build . 
   ```

3. 運行：

   ```bash
   .\bin\Debug\ui_project.exe
   ```

### Linux

1. 安裝 CMake 和 C++ 編譯器（例如 g++）。

   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```

2. 配置並建構：

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

3. 運行：

   ```bash
   ./bin/ui_project
   ```

### macOS

1. 安裝 CMake 和 C++ 編譯器（例如透過 Xcode 安裝 clang）。

   ```bash
   brew install cmake
   ```

2. 配置並建構：

   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

3. 運行：

   ```bash
   ./bin/ui_project
   ```

## 功能

- MVC 模式: 實現 UI（UserModel、UserView）。
- Application 模式: 管理應用生命週期。
- LoggerFacade: 搭配 spdlog 實現控制台和檔案日誌。
- 主題切換：支援深色、淺色和經典主題，使用 RapidJSON 儲存至 config/theme.json。
- 檔案管理器：顯示和管理檔案/資料夾，支援過濾和圖片載入，使用 Emoji 圖標（📁、📄）。
- 使用智能指標確保記憶體安全。

## 日誌輸出

日誌寫入 `logs/app.log`，並在控制台顯示。
