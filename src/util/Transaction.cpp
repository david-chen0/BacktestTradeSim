#include "../include/util/Transaction.hpp"

#include <string>

int Transaction::getNumShares() const {
	return numShares;
}

double Transaction::getPrice() const {
	return price;
}

std::string Transaction::getTimestamp() const {
	return timestamp;
}