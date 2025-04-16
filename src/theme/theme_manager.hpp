#pragma once 


#include "logger/logger_facade.hpp"
#include <string>
#include <memory>

class ThemeManager {
public:

    enum class Theme {
        Dark,
        Light,
        Classic
    };

private:

    Theme current_theme;
    std::shared_ptr<LoggerFacade> logger;
    std::string config_path;

public:

    ThemeManager(std::shared_ptr<LoggerFacade> log, const std::string& path = "config/theme.json");

    void applyTheme(Theme theme);
    void saveTheme();

    inline Theme getCurrentTheme() const { return current_theme; }
};