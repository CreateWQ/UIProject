#include "ui_factory.hpp"

std::unique_ptr<UserView> UIFactory::createUserView(
    std::shared_ptr<UserModel> model,
    std::shared_ptr<LoggerFacade> logger
) {
    return std::make_unique<UserView>(model, logger);
}