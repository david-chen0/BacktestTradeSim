#include "../../include/util/Broker.hpp"

#include <stdexcept>

void Broker::executeTrade(
	int numShares,
	std::string timestamp,
	const Security& security,
	double price
) {
	double totalPrice = numShares * price;

	// Reject the trade if there is not enough capital
	if (totalPrice > portfolio.getBalance()) {
		throw std::invalid_argument("Total price of securities can not exceed portfolio balance.");
	}

	portfolio.adjustBalance(-1 * totalPrice);

	Transaction transaction(
		security,
		numShares,
		price,
		timestamp
	);
	portfolio.addTransaction(security, transaction);
}

void Broker::updateSecurityPrices(const Security& security, double newPrice) {
	securityPrices.insert_or_assign(security, newPrice);
}

double Broker::getSecurityPrice(const Security& security) const {
	return securityPrices.find(security)->second;
}
