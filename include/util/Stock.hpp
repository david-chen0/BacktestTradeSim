#ifndef STOCK_HPP
#define STOCK_HPP

#include Security.hpp
#include <iostream>
#include <format>

/*
* Stock class inherits the Security class.
* 
* [identifier]: Reference to string of the stock identifier(ex: AAPL ticker). The reference is used for memory and performance sake and const is used to ensure the value is never changed.
* [direction]: The SecurityDirection of the stock(long, short).
*/
class Stock : public Security {
public:
    Stock(
        const std::string& identifier,
        SecurityDirection direction,
        double currentPrice
    ): Security(SecurityType::Stock, direction, identifier, currentPrice),
        direction(direction)
    {}

    double getCurrentPrice() const { return currentPrice; }
    void setCurrentPrice(double price) { currentPrice = price; }

    void displayInfo() const override {
        std::cout << std::format("Stock ticker: {}, investment direction: {}, current price: {}.\n",
            identifier,
            directionToString(direction),
            currentPrice
        );
    }

private:
    SecurityDirection direction;
};

#endif