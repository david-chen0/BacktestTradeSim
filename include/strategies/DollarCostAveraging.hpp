#ifndef DOLLARCOSTSTRATEGY_HPP
#define DOLLARCOSTSTRATEGY_HPP

#include "Strategy.hpp"

/*
* DollarCostAveraging class represents the dollar cost averaging(DCA) strategy,
* in which a set amount of money is put into a set of securities each time period.
*
* [income]: This represents the strategy's income, which will be the amount deposited at each interval
* [startDate]: Inherited from Strategy
* [endDate]: Inherited from Strategy
* [securities]: Inherited from Strategy
* [timeInterval]: The time between income/deposits
*/
class DollarCostAveraging : public Strategy {
public:
	DollarCostAveraging(
		int income,
		std::string startDate,
		std::string endDate,
		std::set<Security> securities,
		int timeInterval
	) : Strategy(income, startDate, endDate, securities),
		income(income),
		timeInterval(timeInterval),
		latestTimeProcessed("0")
	{}

	virtual void processDataPoint() = 0;

private:
	int income;
	int timeInterval;
	std::string latestTimeProcessed;
};

#endif