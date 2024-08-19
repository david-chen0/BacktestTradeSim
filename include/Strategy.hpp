#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <set>
#include <string>
#include <vector>

// Declare the Strategy class
/*
* The Strategy class is meant to be the base class that all investment strategies inherit.
* 
* The children strategy classes will contain the logic that determines the position to take on securities given the data.
* 
* TODO: Consider making a Portfolio class that takes over a few things(ex: securities, balance, liquidBalance, currentPosition)
*/
class Strategy {
public:
    // Virtual destructor to ensure that the proper destructor(the inheritor's destructor) is called to prevent memory leaks
    virtual ~Strategy() {}

    // Pure virtual function (making this class abstract)
    virtual void execute() = 0;

    // The methods below are examples for how we could access the securities set
    // Function to add a security to the set
    void addSecurity(const std::string& security) {
        securities.insert(security);
    }

    // Function to remove a security from the set
    void removeSecurity(const std::string& security) {
        securities.erase(security);
    }

    // Function to check if a security is in the set
    bool hasSecurity(const std::string& security) const {
        return securities.find(security) != securities.end();
    }
    // end of securities functions

    /*
    * Takes in a data point and outputs an action(long, short, etc).
    * Implemented as a pure virtual method to force inheriting classes to override this method.
    * 
    * Note that this will likely need to be extended to process multiple data points(ex: from different stocks) to account for some strategies(ex: pairs trading)
    */
    virtual void processData() = 0;

    /*
    * 
    * 
    * @returns Vector<String> containing the current positions of the portfolio
    * PROBABLY WANT TO CHANGE THIS TO A MAP OR SOMETHING SINCE WE WANT TO RETURN THE QUANTITY OF THE POSITION TOO
    */
    std::vector<std::string> currentPosition() const;

    // TODO: make a listTransactions method with the following purpose:
    // listTransactions: Lists all the transactions we've taken for the security(s) provided


protected:
    // Protected constructor to prevent instantiation of the base class
    Strategy() {}

    // Set to store all the securities that were traded, even if their positions were closed
    std::set<std::string> securities;

    /*
    * Need some way to track all positions acquired and closed(ex: hashmap from security name to list of transactions ordered by time the transaction was made)
    * This will require some good memory management and optimization, make it work first then make it good
    */

    // Stores the total net worth of the account
    // TODO: Decide whether this is even necessary since we could just calculate this rather than have to continually maintain it
    int balance;

    // Stores the amount of cash in the account
    int liquidBalance;
};

#endif // STRATEGY_H