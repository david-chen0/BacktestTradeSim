#include "../../include/data/DataFetcher.hpp"
#include "../../include/strategies/DollarCostAveraging.hpp"

#include <cmath>

void DollarCostAveraging::processDataPoint(const Security& security, const SecurityData& securityData, std::string strategyDate) {
	// Only continue if the time interval has passed and we have received our income
	int timeDifference = Strategy::calculateEpochDifference(latestTimeProcessed, strategyDate);
	if (timeDifference < timeIntervalInDays * 86400) {
		return;
	}

	// Fetching the security data for the input day
	std::map<Security, SecurityData> securityDataMap = DataFetcher::fetchData(securities, currentDate);

	// Calculating the total number of shares of each security we can buy, assuming an equal number of shares for each security
	double spendingPower = income + portfolio.getBalance();
	double totalSecurityCosts = 0.0;
	for (const auto& [security, securityData] : securityDataMap) {
		totalSecurityCosts += securityData.getOpen();
	}
	int sharesPerSecurity = static_cast<int>(std::floor(spendingPower / totalSecurityCosts));

	// Executing the trades, which will automatically adjust our portfolio balance
	for (const auto& [security, securityData] : securityDataMap) {
		broker.executeTrade(
			sharesPerSecurity,
			currentDate,
			security,
			securityData.getOpen()
		);
	}
}