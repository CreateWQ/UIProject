#include "ui_factory.hpp"

std::unique_ptr<UserView> UIFactory::createUserView(
    std::shared_ptr<UserModel> model,
    std::shared_ptr<FileManager> file_manager,
    std::shared_ptr<ThemeManager> theme_manager,
    std::shared_ptr<ImageProcessor> image_processor,
    std::shared_ptr<LoggerFacade> logger
) {
    auto view = std::make_unique<UserView>(model, file_manager, theme_manager, image_processor, logger);

    return view;
}