#pragma once 

#include "model/user_model.hpp"
#include "logger/logger_facade.hpp"
#include "theme/theme_manager.hpp"
#include "file/file_manager.hpp"
#include "image_process/image_processor.hpp"
#include <string>
#include <memory>


class UserView : public Observer, public std::enable_shared_from_this<UserView> {
    private:
    
        std::shared_ptr<UserModel> model;
        std::shared_ptr<LoggerFacade> logger;
        std::shared_ptr<FileManager> file_manager;
        std::shared_ptr<ImageProcessor> image_processor;
        std::shared_ptr<ThemeManager> theme_manager;

        std::string inputBuffer;
        std::string file_filter_buffer;

        int selected_theme;
        int selected_log_level;
        int selected_strategy;

        float processing_params[3];

        std::string execute_path;
    public:
    
        UserView(
            std::shared_ptr<UserModel> m, 
            std::shared_ptr<FileManager> file,
            std::shared_ptr<ThemeManager> theme,
            std::shared_ptr<ImageProcessor> image,
            std::shared_ptr<LoggerFacade> log = nullptr
        );
            
        void update() override;
    
        void render();
    
    };