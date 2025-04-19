#pragma once 

#include <spdlog/spdlog.h>
#include <memory>
#include <string> 
#include <queue>
#include <fmt/format.h>

using LogEntry = std::pair<spdlog::level::level_enum, std::string>;

class LoggerFacade {
private:

    std::shared_ptr<spdlog::logger> logger;
    std::vector<LogEntry> log_buffers;

    static constexpr size_t MAX_LOGS = 1000;

public:

    LoggerFacade(const std::string& logger_name = "app_logger");

    ~LoggerFacade();

    template<typename ... Args> 
    inline void error(std::string_view str, Args&&... args) {
        std::string ss = std::vformat(str, std::make_format_args(args ...));
        logger->error(ss);
        log_buffers.push_back({spdlog::level::err, ss});
        if (log_buffers.size() > MAX_LOGS) log_buffers.erase(log_buffers.begin());
    }

    template<typename ... Args> 
    inline void info(std::string_view str, Args&&... args) {
        std::string ss = std::vformat(str, std::make_format_args(args ...));
        logger->info(ss);
        log_buffers.push_back({spdlog::level::info, ss});
        if (log_buffers.size() > MAX_LOGS) log_buffers.erase(log_buffers.begin());
    }

    template<typename ... Args> 
    inline void debug(std::string_view str, Args&&... args) {
        std::string ss = std::vformat(str, std::make_format_args(args ...));
        logger->debug(ss);
        log_buffers.push_back({spdlog::level::debug, ss});
        if (log_buffers.size() > MAX_LOGS) log_buffers.erase(log_buffers.begin());
    }

    inline const std::vector<LogEntry>& getLogBuffers() const { return log_buffers; }

    inline void clearLogBuffers() {
        log_buffers.clear();
        logger->info("Log Buffer is cleared");
    }
    
};
