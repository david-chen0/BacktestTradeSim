#include "../../include/strategies/Strategy.hpp"
#include "../../include/data/DataFetcher.hpp"

#include <format>
#include <iostream>
#include <string>
#include <sstream>

double Strategy::getStrategyValue() const {
	double sum = 0.0;

	// Iterate over the prices we have
	for (const auto& security : securities) {
		double securityPrice = broker.getSecurityPrice(security);
		int positionSize = portfolio.getPositionSize(security);
		std::cout << std::format(
			"Final info for security {}, Price: {}, NumShares: {}\n",
			security.getIdentifier(),
			securityPrice,
			positionSize
		);
		sum += securityPrice * positionSize;
	}

	return sum;
}

void Strategy::evaluatePerformance() const {
	double amountDeposited = portfolio.getTotalDeposited();
	std::cout << "Total amount deposited: " << amountDeposited << std::endl;

	double finalPortfolioValue = getStrategyValue();
	std::cout << "Final strategy value: " << finalPortfolioValue << std::endl;

	double returnRate = ((finalPortfolioValue / amountDeposited) - 1) * 100;
	std::cout << "Return rate: " << returnRate << "%" << std::endl;

	// TODO: Output other metrics, such as portfolio high, portfolio low, volatility, Sharpe ratio, etc
}

void Strategy::runStrategy() {
	// Need this while loop to cURL in batches
	while (latestQueriedDate != endDate) {
		std::cout << "Current query starting from: " << latestQueriedDate << std::endl;

		// Set the next date we will query to to the minimum of the endDate or maxQueryTime from the currentDate
		std::string queryToDate;
		time_t currentTime = stringToTime(latestQueriedDate);
		time_t endTime = stringToTime(endDate);
		time_t effectiveMaxQueryTime = getMaxQueryTime();
		if (endTime - currentTime > effectiveMaxQueryTime) {
			queryToDate = std::to_string(currentTime + effectiveMaxQueryTime);
		}
		else {
			queryToDate = std::to_string(endTime);
		}

		// Get the data and store it in the map
		std::map<Security, std::vector<SecurityData>> securityDataMap = DataFetcher::fetchData(
			securities,
			latestQueriedDate,
			queryToDate
		);

		std::cout << "queryToDate: " << queryToDate << std::endl;
		std::cout << "latestQueriedDate: " << latestQueriedDate << std::endl;
		if (queryToDate == latestQueriedDate) {
			std::cout << "queryToDate and latestQueriedDate are equal, strategy is finished" << std::endl;
			break;
		}

		std::string currentDate = latestQueriedDate;
		std::string nextDate = "N/A";
		// Keep processing data until you reach the new latest queried to date
		std::cout << "Starting to process all the data" << std::endl;
		while (true) {
			std::cout << "Processing with current date " << currentDate << std::endl;
			// List of all the SecurityData for the current day
			std::map<Security, SecurityData> currentSecurityDataMap;
			for (auto& [security, securityDataList] : securityDataMap) {
				SecurityData securityData = securityDataList[0];
				if (securityData.getDate() == currentDate) {
					currentSecurityDataMap.emplace(security, securityData);

					// Adding nextDate == "N/A" check ensures we don't set this more than once
					if (nextDate == "N/A" && securityDataList.size() > 1) {
						nextDate = securityDataList[1].getDate();
					}

					securityDataList.erase(securityDataList.begin());
					broker.updateSecurityPrices(security, securityData.getClose());
				}
			}

			processData(currentSecurityDataMap, currentDate);

			// We are finished if the current date is equal to the query to date
			if (currentDate == queryToDate) {
				break;
			}
			currentDate = nextDate;
			nextDate = "N/A";
		}

		// Updating the latestQueriedDate to the end date of this current query
		latestQueriedDate = queryToDate;
	}

	//	// Fetching the security data for the input day
	//	std::map<Security, SecurityData> securityDataMap = DataFetcher::fetchData(securities, currentDate);

	//for (auto& [security, securityData] : securityDataMap) {
	//	// Invoke each custom Strategy's processDataPoint method on the security and current date
	//	processDataPoint(security, securityData, currentDate);

	//	// Updates the security price with the close price
	//	broker.updateSecurityPrices(security, securityData.getClose());
	//}
}

time_t Strategy::stringToTime(const std::string& epochString) {
	time_t epoch;
	std::istringstream(epochString) >> epoch;
	return epoch;
}

int Strategy::calculateEpochDifference(const std::string& epochStr1, const std::string& epochStr2) {
	// Convert the epoch strings to long long integers
	long long epoch1 = std::stoll(epochStr1);  // std::stoll converts string to long long
	long long epoch2 = std::stoll(epochStr2);

	return static_cast<int>(epoch2 - epoch1);
}

time_t Strategy::getMaxQueryTime() const {
	return maxQueryTime;
}