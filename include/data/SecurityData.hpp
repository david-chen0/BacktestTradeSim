#ifndef SECURITYDATA_HPP
#define SECURITYDATA_HPP

#include <string>
#include <optional>

/*
* Represents the security data retrieved for that day from the DataFetcher.
* 
* [date]: The date of the data, represented as a string epoch
* [high]: The high price of the day, set to optional
* [low]: The low price of the day, set to optional
* [open]: The price at open
* [close]: The price at close
* [adjClose]: The adjusted close price
* [volume]: The volume of shares traded that day
*/
class SecurityData {
public:
    // Constructor
    SecurityData(
        const std::string& date,
        double open,
        double close,
        double adjClose,
        long volume
    ) : date(date),
        open(open),
        close(close),
        adjClose(adjClose),
        volume(volume)
    {}

    // Setters for optional fields
    void setHigh(double h) { high = h; }
    void setLow(double l) { low = l; }

    // Check if optional fields are set
    bool hasHigh() const { return high.has_value(); }
    bool hasLow() const { return low.has_value(); }

    // Getter methods
    const std::string& getDate() const { return date; }
    std::optional<double> getHigh() const { return high; }
    std::optional<double> getLow() const { return low; }
    double getOpen() const { return open; }
    double getClose() const { return close; }
    double getAdjClose() const { return adjClose; }
    long getVolume() const { return volume; }

    // Other methods
    static std::string addDayToEpoch(const std::string& date); // Adds a day to the input epoch and returns the resulting epoch as a string
    static std::string dateToEpoch(const std::string& date); // Converts a date(MM/DD/YYYY) to an epoch string

private:
    std::string date;
    std::optional<double> high;
    std::optional<double> low;
    double open;
    double close;
    double adjClose;
    long volume;
};

#endif