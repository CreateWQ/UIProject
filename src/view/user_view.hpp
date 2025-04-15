#pragma once 

#include "model/user_model.hpp"
#include "logger/logger_facade.hpp"
#include <string>
#include <memory>


class UserView : public Observer, public std::enable_shared_from_this<UserView> {
    private:
    
        std::shared_ptr<UserModel> model;
        std::shared_ptr<LoggerFacade> logger;
        std::string inputBuffer;
    
    public:
    
        UserView(
            std::shared_ptr<UserModel> m, 
            std::shared_ptr<LoggerFacade> log = nullptr
        );
    
        void update() override;
    
        void render();
    
    };