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

	// Gets the value of the strategy's portfolio
	double getStrategyValue() const;

	// Prints some info about the strategy's performance
	void evaluatePerformance() const;

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

	// Gets the maxQueryTime variable. Override this to set a maxQueryTime variable in a strategy.
	virtual time_t getMaxQueryTime() const;


protected:
	// Protected constructor to prevent instantiation of the base class
	Strategy(
		double startingBalance,
		std::string startDate,
		std::string endDate,
		std::set<Security> securities
	) : startDate(startDate),
		latestQueriedDate(startDate),
		endDate(endDate),
		portfolio(Portfolio(startingBalance)),
		broker(Broker(portfolio)),
		securities(securities)
	{}

	// The starting date of the strategy, represented as an epoch string
	std::string startDate;

	// The ending date of the strategy, represented as an epoch string
	std::string endDate;

	// The portfolio storing all the securities that were traded, even if their positions were closed
	Portfolio portfolio;

	// The Broker client used for executing trades
	Broker broker;

	// Set of securities that are to be considered
	std::set<Security> securities;

	// Maximum amount of time difference from startDate to endDate we want in a scan
	// The purpose of this is to set a limit on the amount of data we store in memory
	// Consider making this public so that a strategy can override it
	const time_t maxQueryTime = 31536000; // 1 year

	// Latest date(as an epoch string) that we've queried for data
	std::string latestQueriedDate;

	// NEED TO CHANGE THIS TO PROCESS ALL SECURITIES TOGETHER, NOT JUST ONE SECURITY AT A TIME
	// workflow: call processData, processData either looks for the strategy's currentDate or uses the first date in the jsons,
	// then processes them all together
	virtual void processData(std::map<Security, SecurityData> currentSecurityDataMap, const std::string& currentDate) = 0;

	// Takes in a data point and outputs an action(long, short, etc).
	// Implemented as a pure virtual method to force inheriting classes to override this method.
	//virtual void processData(const Security& security, const SecurityData& securityData, std::string currentDate) = 0;
};

#endif