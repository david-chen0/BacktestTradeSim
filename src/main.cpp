#include "../include/strategies/DollarCostAveraging.hpp"
#include "../include/util/Security.hpp"
#include "../include/util/Stock.hpp"

#include <set>
#include <iostream>

int main(int argc, char* argv[]) {
	// Parameters
	double startingCapital = 1000000; // Starting capital of 1 million
	std::string startDate = "1262611800"; // Jan 4, 2010
	std::string endDate = "1294147800"; // Jan 4, 2011

	Stock spyEtf("SPY");
	std::set<Security> securities{ spyEtf };

	// Income of 10 thousand every 2 weeks
	double income = 10000;
	int timeIntervalInDays = 14;

	// Workflow:
	DollarCostAveraging dcaStrategy(
		startingCapital,
		startDate,
		endDate,
		securities,
		income,
		timeIntervalInDays
	);

	// Running the strategy
	while (dcaStrategy.getCurrentDate() < endDate) {
		dcaStrategy.runStrategy();
	}

	// Printing the results for now, figure out graphing later
	double strategyValue = dcaStrategy.getStrategyValue();
	std::cout << "Strategy value: " << strategyValue << std::endl;

	return 0;
}