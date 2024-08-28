#include "DataFetcher.hpp"

#include <ctime>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Method to build the URL for a given Security's cURL request
/*
* Method to build the URL for a given Security's cURL
* 
* [security]: The security to build the URL for
* [startDate]: The start date represented as an epoch
* [endDate]: The end date represented as an epoch
* 
* @returns URL string to be used for the cURL request
*/
std::string DataFetcher::buildURL(const Security& security, const std::string& startDate, const std::string& endDate) {
    std::string urlBase = "https://query1.finance.yahoo.com/v7/finance/download/" + security.getIdentifier();

    switch (security.getType()) {
        case SecurityType::Stock:
            return urlBase + "?period1=" + startDate + "&period2=" + endDate + "&interval=1d&events=history";
        default:
            throw std::invalid_argument("Unsupported SecurityType for URL building.");
    }
}

// Helper function to handle curl response
/*
* Helper function for cURL to handle HTTP response
* 
* [contents]: Pointer to the raw data received from the HTTP response
* [size]: Size of each element(usually 1 byte for HTTP responses)
* [nmemb]: The number of elements/blocks in the data chunk, with the total size being size * nmemb
* [userp]: Input pointer for where the data should be stored(in this case, it's being stored as a string)
* 
* @returns The number of bytes processed
*/
static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/*
* Sets up and executes an HTTP request using cURL
* 
* [url]: The URL to make the HTTP request for
* 
* @returns String containing the full response data
*/
std::string DataFetcher::performRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Sets the URL string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); // Sets the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Provides a pointer to be passed to the callback function
        res = curl_easy_perform(curl); // Performs the request
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl); // Frees the unused resources
    }
    return readBuffer;
}

// Method to fetch data for a list of Security objects
void DataFetcher::fetchData(const std::vector<Security>& securities, const std::string& date) {
    std::map<Security, std::string>

    std::string currentDay = toMarketOpenString(date);
    std::string nextDay = addDayToEpoch(currentDay);
    for (const auto& security : securities) {
        std::string url = buildURL(security, currentDay, nextDay);
        std::string data = performRequest(url);
    }
    // idea is to return a map from security to the info for the day
    // want to change this method to only accept a single date, compute the end of that day, and then 
}

std::string toMarketOpenString(const std::string& inputEpochStr) {
    // Convert the input string to std::time_t (epoch time)
    std::time_t inputEpoch = std::stoll(inputEpochStr);

    // Convert epoch to tm structure in UTC
    std::tm* timeInfo = std::gmtime(&inputEpoch);

    // Set the time to 9:30 AM EST
    timeInfo->tm_hour = 9 + 5;  // Convert 9:30 AM EST to 9:30 AM UTC
    timeInfo->tm_min = 30;
    timeInfo->tm_sec = 0;

    // Convert the modified tm structure back to epoch
    std::time_t marketOpenEpoch = std::mktime(timeInfo);

    // Adjust for the 5-hour difference between EST and UTC
    marketOpenEpoch -= 5 * 3600;

    // Convert the epoch to a string
    std::tm* marketOpenTm = std::gmtime(&marketOpenEpoch);
    std::ostringstream oss;
    oss << std::put_time(marketOpenTm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}