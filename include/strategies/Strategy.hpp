#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "../util/Portfolio.hpp"
#include "../util/Security.hpp"

#include <set>
#include <string>
#include <vector>

/*
* The Strategy class is meant to be the base class that all investment strategies inherit.
*
* The children strategy classes will contain the logic that determines the position to take on securities given the data.
*
* [startingBalance]: The starting balance of the strategy
* [startDate]: The start date for the strategy
* [endDate]: The end date for the strategy
* [securities]: The set of securities that the strategy will consider
*/
class Strategy {
public:
	// Virtual destructor to ensure that the proper destructor(the inheritor's destructor) is called to prevent memory leaks
	virtual ~Strategy() {}

	// Takes in a data point and outputs an action(long, short, etc).
	// Implemented as a pure virtual method to force inheriting classes to override this method.
	virtual void processDataPoint() = 0;


protected:
	// Protected constructor to prevent instantiation of the base class
	Strategy(
		int startingBalance,
		std::string startDate,
		std::string endDate,
		std::set<Security> securities
	) : totalBalanceInput(startingBalance),
		startDate(startDate),
		endDate(endDate),
		securities(securities)
	{}

	// The portfolio storing all the securities that were traded, even if their positions were closed
	Portfolio portfolio;

	// The total amount of money added to the portfolio, where deposits are positive and withdrawals are negative
	int totalBalanceInput;

	// The starting date of the strategy, represented as an epoch string
	std::string startDate;

	// The ending date of the strategy, represented as an epoch string
	std::string endDate;

	// Lists the securities that are to be considered
	std::set<Security> securities;
};

#endif