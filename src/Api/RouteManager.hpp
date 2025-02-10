#ifndef API_ROUTEMANAGER_H_
#define API_ROUTEMANAGER_H_

#include "Utils/Core/Singleton.hpp"
#include "Utils/Logger/Logger.hpp"
#include <crow.h>

namespace Api {
    class RouteManager : public Utils::Core::Singleton<RouteManager> {
        friend class Utils::Core::Singleton<RouteManager>;
        RouteManager(crow::SimpleApp &app) : app_(app){};

    public:
        void SetupRoutes();

    private:
        crow::SimpleApp &app_;
    };
}  // namespace Api

#endif