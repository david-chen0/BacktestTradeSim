#include "../include/strategies/DollarCostAveraging.hpp"
#include "../include/util/Broker.hpp"
#include "../include/util/Security.hpp"
#include "../include/util/Stock.hpp"

#include <set>
#include <iostream>

int main(int argc, char* argv[]) {
	// TODO: Momentum trading, mean-reversal trading, pairs trading, add options, add financial metrics,
	// make library more generic/easy for other libraries to connect to

	// StrategyContainer Parameters
	double startingBalance = 1000000; // Starting capital of 1 million
	std::string startDate = "1262611800"; // Jan 4, 2010
	std::string endDate = "1294147800"; // Jan 4, 2011
	time_t maxQueryTimeStep = time_t(2628000);

	Stock spyEtf("SPY");
	std::set<Security> securities{ spyEtf };

	StrategyContainer strategyContainer(
		startDate,
		endDate,
		securities,
		maxQueryTimeStep
	);

	// DollarCostAveraging Parameters
	// Income of 10 thousand every 2 weeks
	double income = 10000;
	int timeIntervalInDays = 14;

	// Workflow:
	DollarCostAveraging dcaStrategy(
		startingBalance,
		income,
		timeIntervalInDays
	);

	// Adding the strategy to the strategy container
	strategyContainer.addStrategy(&dcaStrategy);

	// Running the strategy
	strategyContainer.runStrategies();

	strategyContainer.evaluatePerformance(&dcaStrategy);

	return 0;
}