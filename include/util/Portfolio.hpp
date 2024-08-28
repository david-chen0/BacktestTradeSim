#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "Security.hpp"

#include <map>

/*
* The Portfolio class is meant to represent an individual portfolio, which contains information about
* the securities the portfolio holds, the transactions executed in the portfolio, the balance of the
* portfolio, and more.
*/
class Portfolio {
public:
	// Constructor
	Portfolio();

	// Destructor
	~Portfolio();

    // Checks whether our portfolio holds a position in the security provided
    // Returns false if the security is not in the positions map or if there are no shares found for that security, true otherwise
    bool hasPosition(const Security& security);

    // Adds a position to our portfolio
    void addPosition(const Security& security, int numShares);

    // Removes a position from our portfolio
    void removePosition(const Security& security);

    // Returns the number of shares of the security
    int getPositionSize(const Security& security);


private:
    std::map<Security, int> positions; // Maps from security to number of shares for that security, where a negative number indicates a short positon
};

#endif