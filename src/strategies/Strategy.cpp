#include "../../include/strategies/Strategy.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>

time_t Strategy::stringToTime(const std::string& epochString) {
	time_t epoch;
	std::istringstream(epochString) >> epoch;
	return epoch;
}

std::string Strategy::nextBusinessDay(const std::string& epochString) {
	time_t epochTime = stringToEpoch(epochString);

	std::tm* timeInfo = std::gmtime(&epochTime); // Using UTC time
	int dayOfWeek = timeInfo->tm_wday; // Sunday = 0, Monday = 1, .. , Saturday = 6

	// Skip forward only for Friday and Saturday
	if (dayOfWeek >= 5) {
		epochTime += 86400 * (8 - dayOfWeek)
	}
	else {
		epochTime += 86400
	}

	// Convert back to string
	std::ostringstream oss;
	oss << epochTime;
	return oss.str();
}

int Strategy::calculateEpochDifference(const std::string& epochStr1, const std::string& epochStr2) {
	// Convert the epoch strings to long long integers
	long long epoch1 = std::stoll(epochStr1);  // std::stoll converts string to long long
	long long epoch2 = std::stoll(epochStr2);

	return static_cast<int>(epoch2 - epoch1);
}
