#pragma once 

#include <spdlog/spdlog.h>
#include <memory>
#include <string> 


class LoggerFacade {
private:

    std::shared_ptr<spdlog::logger> logger;

public:

    LoggerFacade(const std::string& logger_name = "app_logger");

    ~LoggerFacade();

    template<typename ... Args> 
    inline void error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->error(fmt, std::forward<Args>(args)...);
    }

    template<typename ... Args> 
    inline void info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->info(fmt, std::forward<Args>(args)...);
    }

    template<typename ... Args> 
    inline void debug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        logger->debug(fmt, std::forward<Args>(args)...);
    }
};