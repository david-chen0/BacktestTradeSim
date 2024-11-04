#include "../../include/data/DataFetcher.hpp"
#include "../../include/strategies/DollarCostAveraging.hpp"

#include <cmath>

void DollarCostAveraging::processData(
	std::map<Security, SecurityData> securityDataMap,
	Broker& broker,
	std::string date
) {
	if (securityDataMap.empty()) {
		return;
	}
	
	// Checks if the required amount of time has passed
	int timeDifference = Strategy::calculateEpochDifference(latestTimeProcessed, date);
	if (timeDifference < timeIntervalInDays * 86400) {
		return;
	}

	// Income received after required amount of time has passed
	portfolio.adjustBalance(income, false);

	// Calculating the total number of shares of each security we can buy, assuming an equal number of shares for each security
	double spendingPower = portfolio.getBalance();
	double totalSecurityCosts = 0.0;
	for (auto const& [security, securityData] : securityDataMap) {
		totalSecurityCosts += securityData.getOpen();
	}
	int sharesPerSecurity = static_cast<int>(std::floor(spendingPower / totalSecurityCosts));

	// Executing the trades, which will automatically adjust our portfolio balance
	for (auto const& [security, securityData] : securityDataMap) {
		std::cout << "Executing trade on security " << security.getIdentifier() << " for " << sharesPerSecurity << " shares" << std::endl;
		Broker::executeTrade(
			portfolio,
			sharesPerSecurity,
			date,
			security,
			securityData.getOpen()
		);
	}
}
