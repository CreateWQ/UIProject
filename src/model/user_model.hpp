#pragma once 

#include "logger/logger_facade.hpp"
#include <string>
#include <vector>
#include <memory>

class Observer {
    public:
    
        virtual void update() = 0;
        virtual ~Observer() = default;
    
    };
    
    class UserModel {
    private:
    
        std::string username;
        std::vector<std::weak_ptr<Observer>> observers;
        std::shared_ptr<LoggerFacade> logger;
    
    public:
    
        UserModel(
            const std::string& name = "Guest",
            std::shared_ptr<LoggerFacade> log = nullptr
        );
    
        void setUsername(const std::string& name);
    
        std::string getUsername() const;
    
        void addObserver(std::shared_ptr<Observer> observer);
    
        void notify();
    
    };