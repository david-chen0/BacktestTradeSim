#ifndef DOLLARCOSTSTRATEGY_HPP
#define DOLLARCOSTSTRATEGY_HPP

#include "Strategy.hpp"

/*
* DollarCostAveraging class represents the dollar cost averaging(DCA) strategy,
* in which a set amount of money is put into a set of securities each time period.
* 
* [total
*/
class DollarCostAveraging : public Strategy {
public:
	DollarCostAveraging(
		int totalBalanceInput,
		std::string startDate,
		std::string endDate,
		std::set<Security> securities
	) : Strategy(totalBalanceInput, startDate, endDate, securities) {}
};

#endif