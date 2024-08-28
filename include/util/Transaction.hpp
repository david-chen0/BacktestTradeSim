#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include "Security.hpp"

#include <string>

/*
* Represents a transaction
* 
* [security]: The security in the transaction
* [price]: The price of the transaction
* [time]: The time of the transaction, represented as an epoch
* [numShares]: The number of shares of this transaction, where a negative number represents a sale
*/
class Transaction {
public:
	Transaction(
		const Security& security,
		double price,
		const std::string time,
		int numShares,
	)
};

#endif