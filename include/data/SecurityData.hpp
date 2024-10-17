#ifndef SECURITYDATA_HPP
#define SECURITYDATA_HPP

#include <string>

/*
* Represents the security data retrieved for that day from the DataFetcher.
* 
* [date]: The date of the data, represented as a string epoch
* [high]: The high price of the day
* [low]: The low price of the day
* [open]: The price at open
* [close]: The price at close
* [volume]: The volume of shares traded that day
*/
class SecurityData {
public:
    // Constructor
    SecurityData(
        const std::string& date,
        double open,
        double close,
        double high,
        double low,
        long volume
    ) : date(date),
        open(open),
        close(close),
        high(high),
        low(low),
        volume(volume)
    {}

    // Getter methods
    const std::string& getDate() const { return date; }
    double getHigh() const { return high; }
    double getLow() const { return low; }
    double getOpen() const { return open; }
    double getClose() const { return close; }
    long getVolume() const { return volume; }

    // Other methods
    static std::string addDayToEpoch(const std::string& date); // Adds a day to the input epoch and returns the resulting epoch as a string
    static std::string dateToEpoch(const std::string& date); // Converts a date(MM/DD/YYYY) to an epoch string

private:
    std::string date;
    double high;
    double low;
    double open;
    double close;
    long volume;
};

#endif