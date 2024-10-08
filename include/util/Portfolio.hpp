#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "Security.hpp"
#include "Transaction.hpp"

#include <map>
#include <set>

/*
* The Portfolio class is meant to represent an individual portfolio, which contains information about
* the securities the portfolio holds, the transactions executed in the portfolio, the balance of the
* portfolio, and more.
*/
class Portfolio {
public:
	// Default constructor
	Portfolio() : balance(0) {}

	// Constructor
	Portfolio(
		double balance
	) : balance(balance) {}

	// Destructor
	~Portfolio();

	// Checks whether our portfolio holds a position in the security provided
	// Returns false if the security is not in the positions map or if there are no shares found for that security, true otherwise
	bool hasPosition(const Security& security) const;

	// Adds a position to our portfolio
	void addPosition(const Security& security, int numShares);

	// Removes a position from our portfolio
	void removePosition(const Security& security);

	// Returns the number of shares of the security
	int getPositionSize(const Security& security) const;

	// Adds a transaction to the set of transactions while keeping the transactions set ordered by timestamp
	void addTransaction(const Security& security, const Transaction& transaction);

	// Lists all transactions for the given security
	std::set<Transaction, CompareTransaction> getTransactions(const Security& security) const;

	// Gets the current portfolio balance
	double getBalance() const;

	// Adjusts the balance of the portfolio, where a negative input means to withdraw money
	// If withdrawing leads to a negative balance, then an error will be thrown
	void adjustBalance(double change);


private:
	std::map<Security, int> positions; // Maps from security to number of shares for that security, where a negative number indicates a short positon
	std::map<Security, std::set<Transaction, CompareTransaction>> transactions; // Maps from security to the set of transactions for that security, which is automatically ordered by transaction timestamp
	double balance; // Stores the amount of cash available in the account
};

#endif