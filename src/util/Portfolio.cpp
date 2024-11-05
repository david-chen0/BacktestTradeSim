#include "../include/util/Portfolio.hpp"

#include <format>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

Portfolio::~Portfolio() {}

bool Portfolio::hasPosition(const Security& security) const {
	try {
		return getPositionSize(security) != 0;
	}
	catch (std::runtime_error) { // Catches the runtime error that's thrown if the security is not in the portfolio
		return false;
	}
}

void Portfolio::addPosition(const Security& security, int numShares) {
	positions[security] += numShares;
}

void Portfolio::removePosition(const Security& security) {
	positions.erase(security);
}

int Portfolio::getPositionSize(const Security& security) const {
	auto it = positions.find(security);

	// Checks if the security was found
	if (it != positions.end()) {
		return it->second;
	}
	else {
		throw std::runtime_error("Security not found in the portfolio.");
	}
}

void Portfolio::addTransaction(const Security& security, const Transaction& transaction) {
	transactions[security].insert(transaction);
}

std::set<Transaction, CompareTransaction> Portfolio::getTransactions(const Security& security) const {
	// Find the security in the map
	auto it = transactions.find(security);

	// If found, return the set
	if (it != transactions.end()) {
		return it->second;
	}

	// Otherwise, throw an error
	throw std::runtime_error("Transactions for the specified security not found.");
}

double Portfolio::getBalance() const {
	return balance;
}

double Portfolio::getTotalDeposited() const {
	return totalBalanceDeposited;
}

void Portfolio::adjustBalance(double change, bool isTrade) {
	double newBalance = balance + change;
	if (newBalance < 0) {
		throw std::runtime_error("Can not withdraw more money than there exists in the account.");
	}
	balance = newBalance;
	if (!isTrade) {
		totalBalanceDeposited += change;
	}
}

void Portfolio::takeSnapshot(std::string date) {
	std::cout << "Date:" << date << std::endl;

	if (portfolioSnapshots.find(date) != portfolioSnapshots.end()) {
		std::cout << "Snapshot already exists for date: " << date << std::endl;
		return;
	}
	std::cout << "Snapshot does not exist for date: " << date << std::endl;
	portfolioSnapshots.try_emplace(
		date,
		PortfolioSnapshot(
			balance,
			totalBalanceDeposited,
			positions
		)
	);
	std::cout << "Size of map:" << portfolioSnapshots.size() << std::endl;
}

std::unordered_map<std::string, PortfolioSnapshot> Portfolio::getAllSnapshots() const {
	return portfolioSnapshots;
}
