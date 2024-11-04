#include "../../include/strategies/Strategy.hpp"
#include "../../include/data/DataFetcher.hpp"

#include <format>
#include <iostream>
#include <string>
#include <sstream>

Portfolio Strategy::getPortfolio() const {
	return portfolio;
}

int Strategy::calculateEpochDifference(const std::string& epochStr1, const std::string& epochStr2) {
	// Convert the epoch strings to long long integers
	long long epoch1 = std::stoll(epochStr1);  // std::stoll converts string to long long
	long long epoch2 = std::stoll(epochStr2);

	return static_cast<int>(epoch2 - epoch1);
}

void StrategyContainer::addStrategy(Strategy* strategy) {
	strategies.push_back(strategy);
}

time_t StrategyContainer::stringToTime(const std::string& epochString) {
	time_t epoch;
	std::istringstream(epochString) >> epoch;
	return epoch;
}

double StrategyContainer::getStrategyValue(Strategy* strategy) const {
	double sum = 0.0;
	Portfolio portfolio = strategy->getPortfolio();

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

	return sum + portfolio.getBalance();
}

void StrategyContainer::evaluatePerformance(Strategy* strategy) const {
	Portfolio portfolio = strategy->getPortfolio();

	double amountDeposited = portfolio.getTotalDeposited();
	std::cout << "Total amount deposited: " << amountDeposited << std::endl;

	double finalPortfolioValue = getStrategyValue(strategy);
	std::cout << "Final strategy value: " << finalPortfolioValue << std::endl;

	double returnRate = ((finalPortfolioValue / amountDeposited) - 1) * 100;
	std::cout << "Return rate: " << returnRate << "%" << std::endl;

	// TODO: Output other metrics, such as portfolio high, portfolio low, volatility, Sharpe ratio, etc
}

void StrategyContainer::runStrategies() {
	// The purpose of the while loop is to batch the cURL requests
	while (currentDate != endDate) {
		std::cout << "Current query starting from: " << currentDate << std::endl;

		// Set queryToDate to the next date we will query to, which is the minimum of the endDate and maxQueryTime from the currentDate
		std::string queryToDate;
		time_t currentTime = stringToTime(currentDate);
		time_t endTime = stringToTime(endDate);
		time_t effectiveMaxQueryTime = maxQueryTimeStep;
		if (endTime - currentTime > effectiveMaxQueryTime) {
			queryToDate = std::to_string(currentTime + effectiveMaxQueryTime);
		}
		else {
			queryToDate = std::to_string(endTime);
		}

		// Get the data and store it in the map
		// This method also changes currentDate to the first date in the query and queryToDate to the last date in the query
		std::map<Security, std::vector<SecurityData>> securityDataMap = DataFetcher::fetchData(
			securities,
			currentDate,
			queryToDate
		);

		std::cout << "currentDate: " << currentDate << std::endl;
		std::cout << "queryToDate: " << queryToDate << std::endl;
		if (queryToDate == currentDate) {
			std::cout << "queryToDate and currentDate are equal, strategy is finished" << std::endl;
			break;
		}

		std::string nextDate = "N/A";
		// Keep processing data until you reach the queryToDate
		std::cout << "Starting to process all the data" << std::endl;

		// We must use a while (true) rather than putting the condition in the while loop because the condition
		// is checked after the core logic is finished, not before. Otherwise, the final loop is skipped
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

			for (auto& strategy : strategies) {
				strategy->processData(currentSecurityDataMap, broker, currentDate);
			}

			// We are finished if the current date is equal to the query to date
			if (currentDate == queryToDate) {
				break;
			}
			currentDate = nextDate;
			nextDate = "N/A";
		}

		// Updating the currentDate to the end date of this current query
		currentDate = queryToDate;
	}
}
