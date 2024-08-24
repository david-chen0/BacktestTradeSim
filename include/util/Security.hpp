#ifndef SECURITY_HPP
#define SECURITY_HPP

#include Stock.hpp
#include Option.hpp

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
* [numShares]: The number of shares that the portfolio has of this security. A negative number indicates a short position.
* [identifier]: Reference to string of the security identifier(ex: AAPL ticker for stocks). The reference is used for memory and performance sake and const is used to ensure the value is never changed.
* [currentPrice]: The double representing the current price of the stock.
*/
class Security {
public:
    Security(
        SecurityType type,
        int numShares,
        const std::string& identifier,
        double price
    ): type(type),
        direction(direction),
        identifier(identifier),
        price(price)
    {}

    virtual ~Security() {}

    virtual void displayInfo() const {
        std::cout << std::format("Security type: {}, identifier: {}, investment side: {}, current price: {}.\n",
            typeToString(type),
            identifier,
            getSide(numShares),
            price
        );
    }

    // Methods for SecurityType
    SecurityType getType() const {
        return type;
    }

    // Methods for numShares
    int getNumShares() const {
        return numShares;
    }
    void setNumShares(int newNumShares) {
        numShares = newNumShares;
    }
    void addShares(int newShares) {
        numShares += newShares;
    }
    std::string getSide() const {
        if (numShares > 0) {
            return "Long"
        } else if (numShares < 0) {
            return "Short"
        } else {
            return "None"
        }
    }

    // Methods for identifier
    std::string getIdentifier() const {
        return identifier;
    }

    // Methods for price
    double getCurrentPrice() const {
        return price;
    }
    void setCurrentPrice(double newPrice) {
        price = newPrice;
    }


protected:
    SecurityType type;
    int numShares;
    std::string identifier;
    double price;
};

#endif