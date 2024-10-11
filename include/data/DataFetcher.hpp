#ifndef DATAFETCHER_HPP
#define DATAFETCHER_HPP

#include "../util/Security.hpp"
#include "SecurityData.hpp"

#include <map>
#include <set>
#include <string>

class DataFetcher {
public:
    DataFetcher();

    /*
    * For the given set of securities, returns the info for those securities on the input date
    *
    * [securities]: The securities to processs
    * [date]: The date on which to provide info for
    *
    * @returns Map from the reference of Security to the SecurityData for the day
    */
    static std::map<Security, SecurityData> fetchData(std::set<Security> securities, const std::string& date);

private:
    /*
    * Fetches both the crumb and cookie from Yahoo Finance to authenticate data requests.
    *
    * [crumb]: Reference to a string where the fetched crumb will be stored.
    * [cookie]: Reference to a string where the fetched cookie will be stored.
    *
    * The method makes a cURL request to Yahoo Finance, captures the response body (which contains the crumb),
    * and the response headers (which contain the cookies). It uses helper functions `writeCallback` to process the
    * response body and `headerCallback` to process the response headers.
    *
    * Example call:
    * std::string crumb;
    * std::string cookie;
    * getCrumbAndCookie(crumb, cookie);
    */
    static void getCrumbAndCookie(std::string& crumb, std::string& cookie);

    /*
    * Method to build the URL for a given Security's cURL
    *
    * [security]: The security to build the URL for
    * [startDate]: The start date represented as an epoch
    * [endDate]: The end date represented as an epoch
    *
    * @returns URL string to be used for the cURL request
    */
    static std::string buildURL(const Security& security, const std::string& startDate, const std::string& endDate, const std::string& crumb);

    /*
    * Sets up and executes an HTTP request using cURL
    *
    * [url]: The URL to make the HTTP request for
    * [cookie]: The cookie to make the request with
    *
    * @returns String containing the full response data
    */
    static std::string performRequest(const std::string& url, const std::string& cookie);

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