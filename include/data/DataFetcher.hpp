#ifndef DATAFETCHER_HPP
#define DATAFETCHER_HPP

#include "../util/Security.hpp"
#include "SecurityData.hpp"

#include <string>
#include <vector>
#include <map>

class DataFetcher {
public:
    DataFetcher();
    std::map<const Security&, SecurityData> fetchData(const std::vector<Security&> securities, const std::string& date);

private:
    std::string buildURL(const Security& security, const std::string& startDate, const std::string& endDate);
    std::string performRequest(const std::string& url);
    void processData(const Security& security, const std::string& data);
    SecurityData mapToSecurityData(std::string csvData);
    std::string toMarketOpenString(const std::string& inputEpochStr);
};

#endif