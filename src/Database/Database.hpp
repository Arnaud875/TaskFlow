#ifndef DATABASE_DATABASE_H_
#define DATABASE_DATABASE_H_

#include "Static.hpp"
#include "Utils/Singleton.hpp"

namespace Database {
    class Database : public Utils::Singleton<Database> {
        friend class Utils::Singleton<Database>;
        Database() = default;

    public:
        ~Database() = default;

        bool Connect();
    };
}  // namespace Database

#endif