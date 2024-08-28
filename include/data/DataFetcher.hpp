#ifndef DATAFETCHER_HPP
#define DATAFETCHER_HPP

#include "Security.hpp"

#include <string>
#include <vector>
#include <map>

class DataFetcher {
public:
    DataFetcher();
    void fetchData(const std::vector<Security>& securities, const std::string& startDate, const std::string& endDate);

private:
    std::string buildURL(const Security& security, const std::string& startDate, const std::string& endDate);
    std::string performRequest(const std::string& url);
    void processData(const Security& security, const std::string& data);
};

#endif