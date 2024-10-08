#include "../../include/strategies/Strategy.hpp"
#include "../../include/data/DataFetcher.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>

std::string Strategy::getCurrentDate() const {
	return currentDate;
}

double Strategy::getStrategyValue() const {
	double sum = 0.0;

	// Iterate over the prices we have
	for (const auto& security : securities) {
		sum += broker.getSecurityPrice(security) * portfolio.getPositionSize(security);
	}

	return sum;
}

void Strategy::runStrategy() {
	// Fetching the security data for the input day
	std::map<Security, SecurityData> securityDataMap = DataFetcher::fetchData(securities, currentDate);
	
	for (auto& [security, securityData] : securityDataMap) {
		// Invoke each custom Strategy's processDataPoint method on the security and current date
		processDataPoint(security, securityData, currentDate);

		// Updates the security price with the close price
		broker.updateSecurityPrices(security, securityData.getClose());
	}

	// Advance the current date to the next business day
	currentDate = nextBusinessDay(currentDate);
}

time_t Strategy::stringToTime(const std::string& epochString) {
	time_t epoch;
	std::istringstream(epochString) >> epoch;
	return epoch;
}

std::string Strategy::nextBusinessDay(const std::string& epochString) {
	time_t epochTime = stringToTime(epochString);

	std::tm timeInfo;
	gmtime_s(&timeInfo, &epochTime); // Using UTC time
	int dayOfWeek = timeInfo.tm_wday; // Sunday = 0, Monday = 1, .. , Saturday = 6

	// Skip forward only for Friday and Saturday
	if (dayOfWeek >= 5) {
		epochTime += 86400 * (8 - dayOfWeek);
	}
	else {
		epochTime += 86400;
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
