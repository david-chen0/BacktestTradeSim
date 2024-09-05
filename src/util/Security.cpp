#include "../include/util/Security.hpp"

#include <string>

SecurityType Security::getType() const {
    return type;
}

std::string Security::getIdentifier() const {
    return identifier;
}

double Security::getCurrentPrice() const {
    return price;
}

void Security::setCurrentPrice(double newPrice) {
    price = newPrice;
}