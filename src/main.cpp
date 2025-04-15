#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class LoggerFacade {
private:

    std::shared_ptr<spdlog::logger> logger;

public:

    LoggerFacade(const std::string& logger_name = "app_logger") {
        try {
            // 創建控制台和檔案日誌輸出
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);

            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                "logs/app.log",
                1024 * 1024 * 5, // 5MB per file
                3                // totatlly 3 files
            );
            file_sink->set_level(spdlog::level::info);
            // 創建多輸出日誌器
            logger = std::make_shared<spdlog::logger>(logger_name, spdlog::sinks_init_list{console_sink, file_sink});
            // 設置Debug層級  trace -> debug -> info -> warn -> error -> critical -> off
            logger->set_level(spdlog::level::debug); 
            logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v"); // 自訂格式

            spdlog::register_logger(logger);
            SPDLOG_LOGGER_INFO(logger, "Logger initialized successfully");

        } catch (const spdlog::spdlog_ex& ex) {
            throw std::runtime_error("Logger initialization failed: " + std::string(ex.what()));
        }
    }

    ~LoggerFacade() {
        SPDLOG_LOGGER_INFO(logger, "Logger shutting down");
        spdlog::drop_all();
    }

    template<typename ... Args> 
    void error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->error(fmt, std::forward<Args>(args)...);
    }

    template<typename ... Args> 
    void info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->info(fmt, std::forward<Args>(args)...);
    }

    template<typename ... Args> 
    void debug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->debug(fmt, std::forward<Args>(args)...);
    }
};

class Observer {
public:

    virtual void update() = 0;
    virtual ~Observer() = default;

};

class UserModel {
private:

    std::string username;
    std::vector<std::weak_ptr<Observer>> observers;
    std::shared_ptr<LoggerFacade> logger;

public:

    UserModel(
        const std::string& name = "Guest",
        std::shared_ptr<LoggerFacade> log = nullptr
    ) : username(name), 
        logger(log) 
    {

        if (logger) logger->info("UserModel initialized with username: {}", username);

    }

    void setUsername(const std::string& name) {
        if (logger) logger->info("Updating username from {} to {}", username, name);
        username = name;
        notify();
    }

    std::string getUsername() const {
        return username;
    }

    void addObserver(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
        if (logger) logger->debug("Observer added to UserModel");
    }

    void notify() {
        observers.erase(
            std::remove_if(
                observers.begin(),
                observers.end(),
                [](const std::weak_ptr<Observer>& ptr) {
                    return ptr.expired();
                }
            ),
            observers.end()
        );

        for (const auto& observer : observers) {
            if (auto ptr = observer.lock()) {
                ptr->update();
            }
        }

        if (logger) logger->debug("Notified {} observers", observers.size());
    }

};

class UserView : public Observer, public std::enable_shared_from_this<UserView> {
private:

    std::shared_ptr<UserModel> model;
    std::shared_ptr<LoggerFacade> logger;
    std::string inputBuffer;

public:

    UserView(
        std::shared_ptr<UserModel> m, 
        std::shared_ptr<LoggerFacade> log = nullptr
    ) : model(m), 
        logger(log), 
        inputBuffer(m->getUsername()) 
    {
        if (logger) logger->info("UserView initialized");
    } 

    void update() override {
        inputBuffer = model->getUsername();
        if (logger) logger->debug("UserView updated with new username: {}", inputBuffer);
    }

    void render() {
        ImGui::Begin("User Profile");
        ImGui::Text("Username: %s", model->getUsername().c_str());
        ImGui::InputText("New Username", &inputBuffer[0], inputBuffer.size() + 1);
        if (ImGui::Button("Update")) {
            model->setUsername(inputBuffer);
            if (logger) logger->info("User clicked Updated button");
        }
        ImGui::End();
    }

};

class UIFactory {
public:

    static std::unique_ptr<UserView> createUserView(
        std::shared_ptr<UserModel> model,
        std::shared_ptr<LoggerFacade> logger
    ) {
        return std::make_unique<UserView>(model, logger);
    }

};

class RendererFacade {
private:

    GLFWwindow* window;
    bool initialized;

    void initGLFW() {
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
        window = glfwCreateWindow(1280, 720, "ImGui MVC Demo", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
    }

    void initImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

public:

    RendererFacade() : window(nullptr), initialized(false) {
        initGLFW();
        initImGui();
        initialized = true;
    }

    ~RendererFacade() {
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

    GLFWwindow* getWindow() const { return window; }

    void beginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void endFrame() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.45f, 0.45f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

};

class Application {
private:

    std::unique_ptr<RendererFacade> renderer;
    std::shared_ptr<LoggerFacade> logger;
    std::shared_ptr<UserModel> model;
    std::unique_ptr<UserView> view;

public:

    Application() {
        logger = std::make_shared<LoggerFacade>();
        logger->info("Application starting");

        try {
            renderer = std::make_unique<RendererFacade>();
            logger->info("Renderer initialized");
        } catch (const std::exception& e) {
            logger->error("Renderer initialization failed: {}", e.what());
            throw;
        }

        model = std::make_shared<UserModel>("Alice", logger);
        view = UIFactory::createUserView(model, logger);
    }

    void run() {
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

};

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
