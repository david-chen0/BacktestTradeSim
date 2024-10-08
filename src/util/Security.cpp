#include "../include/util/Security.hpp"

#include <string>

SecurityType Security::getType() const {
    return type;
}

std::string Security::getIdentifier() const {
    return identifier;
}