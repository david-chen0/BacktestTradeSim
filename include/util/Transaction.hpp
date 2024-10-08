#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include "util/Security.hpp"

#include <cstdlib>
#include <string>

/*
* Transaction class represents a transaction for a security
*
* [security]: The security that the transaction was for
* [numShares]: The number of shares traded in this transaction, where a negative number represents a sale
* [timestamp]: The time of the transaction represented as an epoch string
*/
class Transaction {
public:
	Transaction(
		const Security& security,
		int numShares,
		double price,
		std::string timestamp
	) : security(security),
		numShares(numShares),
		price(price),
		timestamp(timestamp)
	{}

	int getNumShares() const;
	double getPrice() const;
	std::string getTimestamp() const;

private:
	const Security& security;
	int numShares;
	double price;
	std::string timestamp;
};

// Comparator for sorting transactions by the string `timestamp` (converted to long long)
struct CompareTransaction {
	bool operator()(const Transaction& lhs, const Transaction& rhs) const {
		return std::stoll(lhs.getTimestamp()) < std::stoll(rhs.getTimestamp());
	}
};

#endif