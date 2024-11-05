#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "../data/SecurityData.hpp"
#include "../util/Broker.hpp"
#include "../util/Portfolio.hpp"
#include "../util/Security.hpp"

#include <ctime>
#include <map>
#include <set>
#include <stdexcept>
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

	// Gets the portfolio
	Portfolio getPortfolio();

	// Given two epoch strings, finds the difference epochStr2 - epochStr1 and returns it as an int
	static int calculateEpochDifference(const std::string& epochStr1, const std::string& epochStr2);

	/*
	* Method for processing the data that must be overriden by strategies.
	*
	* [currentSecurityDataMap]: The current day's map from Security to SecurityData
	* [broker]: The Broker which holds all the prices. Change this when prices are migrated away from broker
	* [date]: The date which the data corresponds to
	*/
	virtual void processData(
		std::map<Security, SecurityData> currentSecurityDataMap,
		Broker& broker,
		std::string date
	) {
		throw std::logic_error("processData method not overridden");
	};

	// Name of the strategy
	const std::string strategyName;

protected:
	// Protected constructor to prevent instantiation of the base class
	Strategy(
		double startingBalance,
		std::string strategyName
	) : portfolio(Portfolio(startingBalance)),
		strategyName(strategyName)
	{}

	// The portfolio storing all the securities that were traded, even if their positions were closed
	Portfolio portfolio;
};

/*
* The StrategyContainer class is meant to contain multiple strategies and then run them at the same time using the same info,
* with the only difference being each individual strategy's logic.
* 
* [startDate]: The start date for the strategy
* [endDate]: The end date for the strategy
* [maxQueryTimeStep]: The maximum amount of time for each cURL request to contain. Default set to 1 year.
* [securities]: The set of securities that the strategy will consider
*/
class StrategyContainer {
public:
	StrategyContainer(
		std::string startDate,
		std::string endDate,
		std::set<Security> securities,
		time_t maxQueryTimeStep = time_t(31536000)
	) : startDate(startDate),
		currentDate(startDate),
		endDate(endDate),
		securities(securities),
		maxQueryTimeStep(maxQueryTimeStep)
	{}

	// Adds a strategy to the vector of strategies. Note that this does not check if the strategy already exists in the vector
	void addStrategy(Strategy* strategy);

	// Converts a string representing epoch time to a time_t object
	static time_t stringToTime(const std::string& epochString);

	// Gets the total value of a strategy's portfolio
	double getStrategyValue(Strategy* strategy) const;

	// Prints some info about a strategy's performance
	void evaluatePerformance(Strategy* strategy) const;

	// Prints all the snapshots of a strategy's portfolio
	void getStrategyPortfolioSnapshots(Strategy* strategy) const;

	// Method to run all the strategies provided simultaneously.
	// TODO: Likely want to make a method that is like runStrategiesUntil which will run strategies to an input time(less than endDate and more than currentDate)
	void runStrategies();

private:
	// This is initialized with the default constructor
	Broker broker;

	// Vector of all the strategies to run
	std::vector<Strategy*> strategies;

	std::string startDate;

	std::string currentDate;

	std::string endDate;

	// Set of all securities to consider across all strategies
	std::set<Security> securities;

	// Maximum amount of time difference from startDate to endDate we want in a scan
	// The purpose of this is to set a limit on the amount of data we store in memory
	const time_t maxQueryTimeStep = 31536000; // 1 year
};

#endif