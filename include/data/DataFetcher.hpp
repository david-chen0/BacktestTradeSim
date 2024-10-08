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
    static std::map<Security, SecurityData> fetchData(std::set<Security> securities, const std::string& date);

private:
    static std::string buildURL(const Security& security, const std::string& startDate, const std::string& endDate);
    static std::string performRequest(const std::string& url);
    static SecurityData mapToSecurityData(std::string csvData);
    static std::string toMarketOpenString(const std::string& inputEpochStr);

    // NEED A METHOD TO GET THE COOKIES WHICH WILL BE USED FOR THE CURL REQUEST
};

#endif