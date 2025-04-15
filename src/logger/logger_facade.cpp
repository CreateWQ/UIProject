#include "logger_facade.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <stdexcept>

LoggerFacade::LoggerFacade(const std::string& logger_name){
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

LoggerFacade::~LoggerFacade() {
    SPDLOG_LOGGER_INFO(logger, "Logger shutting down");
    spdlog::drop_all();
}
