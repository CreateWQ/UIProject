#include "model/user_model.hpp"

UserModel::UserModel(
    const std::string& name,
    std::shared_ptr<LoggerFacade> log
) : username(name), 
    logger(log) 
{

    if (logger) logger->info("UserModel initialized with username: {}", username);

}

void UserModel::setUsername(const std::string& name) {
    if (logger) logger->info("Updating username from {} to {}", username, name);
    username = name;
    notify();
}

std::string UserModel::getUsername() const {
    return username;
}

void UserModel::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
    if (logger) logger->debug("Observer added to UserModel");
}

void UserModel::notify() {
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
