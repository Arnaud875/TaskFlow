#ifndef DATABASE_DATABASEHELPER_H_
#define DATABASE_DATABASEHELPER_H_

#include "Database.hpp"
#include "Static.hpp"

namespace Database {
    /**
     * @brief Format the attributes for the SQL query
     * @param attributes The attributes to format
     * @return A pair containing the formatted attributes and the indexes of the
     * attributes
     */
    std::pair<std::string, std::string>
    FormatAttributes(const std::vector<SQLAttribute> &attributes, bool isSet = false);
}  // namespace Database

#endif