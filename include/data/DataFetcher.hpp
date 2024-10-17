#ifndef DATAFETCHER_HPP
#define DATAFETCHER_HPP

#include "../util/Security.hpp"
#include "SecurityData.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

class DataFetcher {
public:
    DataFetcher();

    /*
    * For the given set of securities, returns the info for those securities between the start and end date.
    * Constraints on the time between start and end date are to be set in strategy.
    *
    * [securities]: The securities to processs
    * [startDate]: The starting date to query for, represented as an epoch string. This will be modified to be the first
    * date actually returned by the queries(ex: if input is "10" but first date processed is "15", this value
    * will be modified to be "15"
    * [endDate]: The end date to query for, represented as an epoch string. This will be modified to be the last date
    * actually returned, similar to startDate(ex: if input is "15" but last date processed is "10", this value will
    * be modified to be "10"
    *
    * @returns Map from security to its map from date(as an epoch string) to that day's security data
    */
    static std::map<Security, std::vector<SecurityData>> fetchData(
        std::set<Security> securities,
        std::string& startDate,
        std::string& endDate
    );

    // NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
    // Name of file where we store the cookie
    //static inline const std::string cookieFile = "../../../../src/data/cookie.txt";

    // Name of the file where we store the crumb
    //static inline const std::string crumbFile = "../../../../src/data/crumb.txt";

    // Location of the folder where we store all the fetched data
    static inline const std::string fetchedDataFolder = "../../../../src/data/fetchedData";

    static const std::string getDataLocation(std::string identifier);

private:
    /*
    * Sets up and executes an HTTP request using cURL
    *
    * [url]: The URL to make the HTTP request for
    * [startDate]: The starting date to query for, represented as an epoch string
    * [endDate]: The end date to query for, represented as an epoch string.
    *
    * @returns String containing the full response data
    */
    static void performRequest(
        const Security& security,
        const std::string& startDate,
        const std::string& endDate
    );

    /*
    * Transforms the received data into a SecurityData format
    *
    * [csvData]: The received CSV data from the cURL request
    *
    * @returns: SecurityData object representing the data from the cURL request
    */
    static SecurityData mapToSecurityData(std::string csvData);

    // Given an input epoch time, returns the epoch for that day's market open time
    static std::string toMarketOpenString(const std::string& inputEpochStr);
};

#endif