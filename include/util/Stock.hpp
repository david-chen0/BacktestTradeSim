#ifndef STOCK_HPP
#define STOCK_HPP

#include "Security.hpp"

#include <string>

/*
* Stock class inherits the Security class.
* The Stock class does not have any extra info needed, as the Security class covers all the required info for stocks.
*
* TODO: Add info such as financial metrics(P/E, volatility, etc)
*/
class Stock : public Security {
public:
	Stock(
		const std::string& identifier
	) : Security(SecurityType::Stock, identifier) {}
};

#endif