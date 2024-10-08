#ifndef SECURITY_HPP
#define SECURITY_HPP

#include <ctime>
#include <format>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Currently only supporting stocks, options, and ETFs
// Other classes for future implementation: Futures, Bonds, Swaps, etc
enum class SecurityType {
	ETF,
	Option,
	Stock
};
// Function to convert SecurityType to string
inline std::string typeToString(SecurityType type) {
	switch (type) {
	case SecurityType::Stock: return "Stock";
	case SecurityType::Option: return "Option";
	case SecurityType::ETF: return "ETF";
	default:
		std::ostringstream oss;
		oss << "Unsupported SecurityType: " << static_cast<int>(type);
		throw std::invalid_argument(oss.str());
	}
}


/*
* Security is the base class for all securities, such as stocks, options, ETFs, and more.
*
* [type]: The SecurityType that this security represents.
* [identifier]: Reference to string of the security identifier(ex: AAPL ticker for stocks). The reference is used for memory and performance sake and const is used to ensure the value is never changed.
* [currentPrice]: The double representing the current price of the stock.
*/
class Security {
public:
	Security(
		SecurityType type,
		const std::string& identifier
	) : type(type),
		identifier(identifier)
	{}

	virtual ~Security() {}

	// Prints the info for the security
	virtual void displayInfo() const {
		std::cout << std::format("Security type: {}, identifier: {}.\n",
			typeToString(type),
			identifier
		);
	}

	// Gets the security type
	SecurityType getType() const;

	// Gets the security identifier
	std::string getIdentifier() const;

	// Defines how two Security objects should be compared, which is used for sets, maps, and more
	bool operator<(const Security& other) const {
		return this->identifier < other.identifier;
	}


protected:
	SecurityType type;
	std::string identifier;
	double price;
};

#endif