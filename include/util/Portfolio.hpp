#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "Security.hpp"
#include "Transaction.hpp"

#include <map>
#include <set>
#include <unordered_map>

/*
* This class represents a snapshot of our Portfolio at the given time.
*/
class PortfolioSnapshot {
public:
	PortfolioSnapshot(
		double balance,
		double totalBalanceDeposited,
		std::map<Security, int> positions
	) : balance(balance),
		totalBalanceDeposited(totalBalanceDeposited),
		positions(positions)
	{};

	//std::string timestamp; // The timestamp of this snapshot THIS SHOULD BE CAPTURED IN THE MAP RATHER THAN STORED HERE FOR MEMORY PURPOSES
	double balance; // The cash balance of the Portfolio at this time
	double totalBalanceDeposited; // Stores the net amount of money deposited into the account(ex: depositing $10 5 times gives 50.0) at this time
	std::map<Security, int> positions; // The positions held by the Portfolio at this time
};

/*
* The Portfolio class is meant to represent an individual portfolio, which contains information about
* the securities the portfolio holds, the transactions executed in the portfolio, the balance of the
* portfolio, and more.
*/
class Portfolio {
public:
	// Default constructor
	Portfolio() : balance(0),
		totalBalanceDeposited(0)
	{}

	// Constructor
	Portfolio(
		double balance
	) : balance(balance),
		totalBalanceDeposited(balance)
	{}

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

	// Gets the total deposited amount
	double getTotalDeposited() const;

	// Adjusts the balance of the portfolio, where a negative input means to withdraw money
	// If withdrawing leads to a negative balance, then an error will be thrown
	// If the call is not for a trade, then it will be assumed that the balance was changed with a manual withdraw/deposit
	void adjustBalance(double change, bool isTrade = true);

	// Takes a snapshot of the portfolio and adds it to the portfolioSnapshots map
	void takeSnapshot(std::string date);

	// Returns all snapshots
	std::unordered_map<std::string, PortfolioSnapshot> getAllSnapshots() const;


private:
	std::map<Security, int> positions; // Maps from security to number of shares for that security, where a negative number indicates a short positon
	std::map<Security, std::set<Transaction, CompareTransaction>> transactions; // Maps from security to the set of transactions for that security, which is automatically ordered by transaction timestamp
	double balance; // Stores the amount of cash available in the account
	double totalBalanceDeposited; // Stores the net amount of money deposited into the account(ex: depositing $10 5 times gives 50.0)
	std::unordered_map<std::string, PortfolioSnapshot> portfolioSnapshots; // Map from date to snapshot of the portfolio
};

#endif