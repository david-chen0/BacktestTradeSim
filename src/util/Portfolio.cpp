#include "Portfolio.hpp"

#include <format>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

bool hasPosition(const Security& security) {
    try {
        return getPositionSize(security) != 0;
    } catch { // Catches the runtime error that's thrown if the security is not in the portfolio
        return false;
    }
}

void addPosition(const Security& security, int numShares) {
    positions[security] = numShares;
}

void removePosition(const Security& security) {
    positions.erase(security);
}

int getPositionSize(const Security& security) {
    auto it = positions.find(security);

    // Checks if the security was found
    if (it != positions.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Security not found in the portfolio.");
    }
}