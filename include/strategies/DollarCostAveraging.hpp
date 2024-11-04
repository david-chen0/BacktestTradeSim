#ifndef DOLLARCOSTSTRATEGY_HPP
#define DOLLARCOSTSTRATEGY_HPP

#include "Strategy.hpp"
#include "../util/Broker.hpp"

/*
* DollarCostAveraging class represents the dollar cost averaging(DCA) strategy,
* in which a set amount of money is put into a set of securities each time period.
*
* [income]: This represents the strategy's income, which will be the amount deposited at each interval
* [startDate]: Inherited from Strategy
* [endDate]: Inherited from Strategy
* [securities]: Inherited from Strategy
* [timeIntervalInDays]: The time between income/deposits in number of days
*/
class DollarCostAveraging : public Strategy {
public:
	DollarCostAveraging(
		double startingBalance,
		double income,
		int timeIntervalInDays
	) : Strategy(startingBalance),
		income(income),
		timeIntervalInDays(timeIntervalInDays),
		latestTimeProcessed("0")
	{}

	void processData(
		std::map<Security, SecurityData> securityDataMap,
		Broker& broker,
		std::string date
	) override;

private:
	double income;
	int timeIntervalInDays;
	std::string latestTimeProcessed;
};

#endif