#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

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

    // Method to add a security to the set
    void addSecurity(const std::string& security) {
        securities.insert(security);
    }

    // Method to remove a security from the set
    void removeSecurity(const std::string& security) {
        securities.erase(security);
    }

    // Method to check if a security is in the set
    bool hasSecurity(const std::string& security) const {
        return securities.find(security) != securities.end();
    }


};

#endif