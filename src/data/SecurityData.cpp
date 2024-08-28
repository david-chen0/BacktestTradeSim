#include "SecurityData.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Adds a day to the input epoch and returns the resulting epoch as a string
static SecurityData::std::string addDayToEpoch(const std::string& date) {
    // Convert the input string to time_t (epoch time in seconds)
    time_t epochTime = std::stol(date);

    // Add the number of seconds in a day (86400 seconds)
    epochTime += 86400;

    // Convert the new time_t value back to a string
    std::ostringstream oss;
    oss << epochTime;
    return oss.str();
}

// Converts date string(format MM/DD/YYYY) to an epoch string
static SecurityData::std::string dateToEpoch(const std::string& date) {
    // Parse the date string (expected format: MM/DD/YYYY)
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%m/%d/%Y");

    if (ss.fail()) {
        throw std::invalid_argument("Failed to parse date: " + date);
    }

    // Convert to epoch time (time since 1970-01-01 00:00:00 UTC)
    std::time_t epochTime = std::mktime(&tm);

    // Convert epoch time to string
    return std::to_string(epochTime);
}