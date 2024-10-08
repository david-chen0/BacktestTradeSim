#ifndef BROKER_HPP
#define BROKER_HPP

#include "./Portfolio.hpp"
#include "./Security.hpp"
#include "./Transaction.hpp"
#include "../data/SecurityData.hpp"

#include <map>
#include <string>

/*
* The Broker class is meant to act as a broker, as it should take in a portfolio and provide all the supporting actions needed for that portfolio.
* This includes, but is not limited to, executing trades, storing transactions in the portfolio, and more.
*
* [portfolio]: The portfolio for this Broker class to manage
*/
class Broker {
public:
	Broker(
		Portfolio& portfolio
	) : portfolio(portfolio) {}

	/*
	* Executes a trade for the given security with the current securityData for the provided portfolio. The transaction is recorded and stored in the portfolio.
	*
	* [numShares]: The number of shares for this trade. A negative number indicates selling/short.
	* [timestamp]: The timestamp of the trade, represented as an epoch string
	* [security]: The security to execute the trade for
	* [price]: The current price of the security
	*/
	void executeTrade(
		int numShares,
		std::string timestamp,
		const Security& security,
		double price
	);

	void updateSecurityPrices(const Security& security, double newPrice);

	double getSecurityPrice(const Security&) const;

private:
	Portfolio& portfolio;
	std::map<Security, double> securityPrices;
};

#endif