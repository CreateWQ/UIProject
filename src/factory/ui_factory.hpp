#pragma once 

#include "model/user_model.hpp"
#include "view/user_view.hpp"
#include "logger/logger_facade.hpp"
#include <memory>

class UIFactory {
public:

    static std::unique_ptr<UserView> createUserView(
        std::shared_ptr<UserModel> model,
        std::shared_ptr<LoggerFacade> logger
    );

};