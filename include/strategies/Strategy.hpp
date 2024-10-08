#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "../data/SecurityData.hpp"
#include "../util/Broker.hpp"
#include "../util/Portfolio.hpp"
#include "../util/Security.hpp"

#include <ctime>
#include <map>
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

	// Retrieves the current date
	std::string getCurrentDate() const;

	// Gets the value of the strategy's portfolio
	double getStrategyValue() const;

	// Method to run the strategy by calling processDataPoint
	void runStrategy();

	// Updates the price of the passed in Security to the input price, which is normally the close price
	void updateSecurityPrice(Security& security, double price);

	// Converts a string representing epoch time to a time_t object
	static time_t stringToTime(const std::string& epochString);

	// Increments by 1 day, skipping weekends if needed, to return the next business day. Does not consider holidays/closed market days.
	static std::string nextBusinessDay(const std::string& epochString);

	// Given two epoch strings, finds the difference epochStr2 - epochStr1 and returns it as an int
	static int calculateEpochDifference(const std::string& epochStr1, const std::string& epochStr2);


protected:
	// Protected constructor to prevent instantiation of the base class
	Strategy(
		double startingBalance,
		std::string startDate,
		std::string endDate,
		std::set<Security> securities
	) : startDate(startDate),
		endDate(endDate),
		currentDate(startDate),
		portfolio(Portfolio(startingBalance)),
		broker(Broker(portfolio)),
		securities(securities)
	{}

	// The starting date of the strategy, represented as an epoch string
	std::string startDate;

	// The ending date of the strategy, represented as an epoch string
	std::string endDate;

	// The current date of the strategy, represented as an epoch string. This will start as startDate
	std::string currentDate;

	// The portfolio storing all the securities that were traded, even if their positions were closed
	Portfolio portfolio;

	// The Broker client used for executing trades
	Broker broker;

	// Set of securities that are to be considered
	std::set<Security> securities;

	// Takes in a data point and outputs an action(long, short, etc).
	// Implemented as a pure virtual method to force inheriting classes to override this method.
	virtual void processDataPoint(const Security& security, const SecurityData& securityData, std::string currentDate) = 0;
};

#endif