#ifndef SECURITY_HPP
#define SECURITY_HPP

#include Stock.hpp
#include Option.hpp

#include <string>
#include <iostream>
#include <ctime>

// Currently only supporting stocks and options
// Other classes for future implementation: Futures, ETFs, Bonds, etc
enum class SecurityType {
    Stock,
    Option
};

// Represents the direction of the security
enum SecurityDirection {
    Long,
    Short
};
// Function to convert SecurityDirection to string
inline std::string directionToString(SecurityDirection dir) {
    return (dir == SecurityDirection::Long) ? "Long" : "Short";
}


/*
* 
* 
* [type]: The SecurityType that this security represents.
* [direction]: The SecurityDirection of the security. This is chosen to be created within the Security class for ease of management in the Portfolio class.
* [identifier]: Reference to string of the security identifier(ex: AAPL ticker for stocks). The reference is used for memory and performance sake and const is used to ensure the value is never changed.
* [currentPrice]: The double representing the current price of the stock.
*/
class Security {
public:
    Security(
        SecurityType type,
        SecurityDirection direction,
        const std::string& identifier,
        double currentPrice
    ): type(type),
        direction(direction),
        identifier(identifier),
        currentPrice(currentPrice)
    {}

    virtual ~Security() {}

    SecurityType getType() const { return type; }
    std::string getIdentifier() const { return identifier; }

    virtual void displayInfo() const {
        std::cout << "Security: " << identifier << "\n";
    }

protected:
    SecurityType type;
    SecurityDirection direction;
    std::string identifier;
    double currentPrice;
};

#endif