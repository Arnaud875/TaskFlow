#include "DatabaseHelper.hpp"
#include <map>

std::pair<std::string, std::string>
Database::FormatAttributes(const std::vector<SQLAttribute> &attributes, bool isSet) {
    std::string attr;
    std::string valueAttr;

    // Pre-allocate
    attr.reserve(attributes.size() * 10);

    if (!isSet) {
        valueAttr.reserve(attributes.size() * 3);

        attr = '(';
        valueAttr = '(';

        // Format the attributes
        const auto lastIndex = attributes.size() - 1;
        for (std::size_t i = 0; i < lastIndex; ++i) {
            attr += attributes[i].name + ", ";
            valueAttr += "?, ";
        }

        // Add the last attribute
        if (!attributes.empty()) {
            attr += attributes[lastIndex].name;
            valueAttr += "?";
        }

        attr += ')';
        valueAttr += ')';

        return {std::move(attr), std::move(valueAttr)};
    } else {
        // Format the attributes
        const auto lastIndex = attributes.size() - 1;
        for (std::size_t i = 0; i < lastIndex; ++i) {
            attr += attributes[i].name + " = ?, ";
        }

        // Add the last attribute
        if (!attributes.empty()) {
            attr += attributes[lastIndex].name + " = ?";
        }

        return {std::move(attr), ""};
    }
}