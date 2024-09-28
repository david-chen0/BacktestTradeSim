#ifndef DATAFETCHER_HPP
#define DATAFETCHER_HPP

#include "../util/Security.hpp"
#include "SecurityData.hpp"

#include <set>
#include <string>
#include <map>

class DataFetcher {
public:
    DataFetcher();
    static std::map<Security, SecurityData> fetchData(const std::set<Security> securities, const std::string& date);

private:
    static std::string buildURL(const Security& security, const std::string& startDate, const std::string& endDate);
    static std::string performRequest(const std::string& url);
    static SecurityData mapToSecurityData(std::string csvData);
    static std::string toMarketOpenString(const std::string& inputEpochStr);
};

#endif