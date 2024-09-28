#include "../../include/data/DataFetcher.hpp"
#include "../../include/strategies/DollarCostAveraging.hpp"

void DollarCostAveraging::processDataPoint() {
	// Only continue if the time interval has passed and we have received our income
	int timeDifference = Strategy::calculateEpochDifference(latestTimeProcessed, currentDate);
	if (timeDifference < timeInterval) {
		return;
	}

	// Fetching the security data for the input day
	std::map<Security, SecurityData> securityDataMap = DataFetcher::fetchData(securities, currentDate);

	int spendingPower = income + totalBalanceInput;

	// get the sum of the prices of all the securities in this strategy and figure out how much of each you can buy
	// might want to balance s.t. each security gets the same amount of $

	// Set the totalBalanceInput to the remaining balance
	totalBalanceInput = spendingPower;

	// Set currentDate to the next business day now that we've finished processing this day
	currentDate = nextBusinessDay(currentDate);
}