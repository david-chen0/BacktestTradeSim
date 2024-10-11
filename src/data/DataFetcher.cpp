#include "../include/data/DataFetcher.hpp"
#include "../include/util/Security.hpp"
#include "../include/data/SecurityData.hpp"

#include <ctime>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

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
* Helper function for cURL to handle HTTP headers
*
* [contents]: Pointer to the raw header data received from the HTTP response
* [size]: Size of each element in the header data block
* [nmemb]: The number of elements/blocks in the data chunk, with the total size being size * nmemb
* [userp]: Input pointer for where the data should be stored (in this case, it's being stored as a string)
*
* @returns The number of bytes processed
*/
static size_t headerCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void DataFetcher::getCrumbAndCookie(std::string& crumb, std::string& cookie) {
	CURL* curl;
	CURLcode res;
	std::string response;
	std::string headers;

	curl = curl_easy_init();
	if (curl) {
		// URL to get the crumb (this URL works for Yahoo Finance crumb request)
		curl_easy_setopt(curl, CURLOPT_URL, "https://query1.finance.yahoo.com/v1/test/getcrumb");

		// Set callback function to capture the response body (crumb)
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// Set callback function to capture the response headers (cookies)
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);

		// Enable cookie handling
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

		// Perform the request
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
		}

		// Clean up
		curl_easy_cleanup(curl);

		// Extract the crumb from the response body
		std::regex crumbRegex(R"([a-zA-Z0-9%]+)");
		std::smatch match;
		if (std::regex_search(response, match, crumbRegex)) {
			crumb = match.str(0); // The first match should be the crumb
		}

		// Extract the cookie from the response headers
		std::regex cookieRegex(R"(set-cookie: ([^;]+))", std::regex_constants::icase);
		std::smatch cookieMatch;
		if (std::regex_search(headers, cookieMatch, cookieRegex)) {
			cookie = cookieMatch.str(1); // Capture the first cookie (usually the one you need)
		}
	}
}

std::string DataFetcher::buildURL(
	const Security& security,
	const std::string& startDate,
	const std::string& endDate,
	const std::string& crumb
) {
	std::string urlBase = "https://query1.finance.yahoo.com/v7/finance/download/" + security.getIdentifier();

	switch (security.getType()) {
	case SecurityType::Stock:
		urlBase += "?period1=" + startDate + "&period2=" + endDate + "&interval=1d&events=history";
		break;
	default:
		throw std::invalid_argument("Unsupported SecurityType for URL building.");
		break;
	}

	urlBase += "&crumb=" + crumb;

	return urlBase;
}

std::string DataFetcher::performRequest(const std::string& url, const std::string& cookie) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		std::cout << "cURLing at URL: " << url << std::endl;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Sets the URL string
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); // Sets the callback function
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Provides a pointer to be passed to the callback function

		// Setting the cookie using CURLOPT_COOKIE
		std::string cookieHeader = "Cookie: " + cookie;
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookieHeader.c_str());

		res = curl_easy_perform(curl); // Performs the request
		if (res != CURLE_OK) {
			std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
		}
		curl_easy_cleanup(curl); // Frees the unused resources
	}
	return readBuffer;
}

std::map<Security, SecurityData> DataFetcher::fetchData(std::set<Security> securities, const std::string& date) {
	std::map<Security, SecurityData> securityDataMap;

	// Figure out what happens when we try to fetch data on days where the market is not open
	std::cout << "Processing date: " << date << std::endl;

	std::string cookie;
	std::string crumb;
	getCrumbAndCookie(cookie, crumb);
	// remove these after debugging, currently cookie is giving "Too" and crumb is empty
	std::cout << "Cookie: " << cookie << std::endl;
	std::cout << "Crumb: " << crumb << std::endl;

	std::string currentDay = toMarketOpenString(date);
	std::string nextDay = SecurityData::addDayToEpoch(currentDay);
	for (auto& security : securities) {
		std::string url = buildURL(security, currentDay, nextDay, crumb);
		std::string data = performRequest(url, cookie);
		SecurityData securityData = mapToSecurityData(data);
		securityDataMap.insert({ security, securityData });
	}

	return securityDataMap;
}

SecurityData DataFetcher::mapToSecurityData(std::string csvData) {
	std::istringstream stream(csvData);
	std::string line;

	// Skip the first line (header)
	std::getline(stream, line);

	// Process the first line of actual data
	if (std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string date, openStr, highStr, lowStr, closeStr, adjCloseStr, volumeStr;

		// Parse each field
		std::getline(lineStream, date, ',');
		std::getline(lineStream, openStr, ',');
		std::getline(lineStream, highStr, ',');
		std::getline(lineStream, lowStr, ',');
		std::getline(lineStream, closeStr, ',');
		std::getline(lineStream, adjCloseStr, ',');
		std::getline(lineStream, volumeStr, ',');

		// Convert strings to appropriate types
		double open = std::stod(openStr);
		double high = std::stod(highStr);
		double low = std::stod(lowStr);
		double close = std::stod(closeStr);
		double adjClose = std::stod(adjCloseStr);
		long volume = std::stol(volumeStr);

		// Create SecurityData object
		SecurityData securityData(date, open, close, adjClose, volume);

		// Set the high and low since they are optional fields
		securityData.setHigh(high);
		securityData.setLow(low);

		// Return the first data line
		return securityData;
	}

	// Handle case if no data lines are present
	throw std::runtime_error("No data found in CSV");
}

std::string DataFetcher::toMarketOpenString(const std::string& inputEpochStr) {
	// Convert the input string to std::time_t (epoch time)
	std::time_t inputEpoch = std::stoll(inputEpochStr);

	// Convert epoch to tm structure in UTC
	// gmtime_s is used rather than gmtime since gmtime_s is thread-safe and does not have race conditions
	std::tm timeStruct;
	std::tm* timeInfo = &timeStruct;
	errno_t err = gmtime_s(timeInfo, &inputEpoch);
	if (err != 0) {
		std::cerr << "Error converting inputEpoch with gmtime_s." << std::endl;
	}

	// Set the time to 9:30 AM EST
	timeInfo->tm_hour = 9 + 5;  // Convert 9:30 AM EST to 9:30 AM UTC
	timeInfo->tm_min = 30;
	timeInfo->tm_sec = 0;

	// Convert the modified tm structure back to epoch
	std::time_t marketOpenEpoch = std::mktime(timeInfo);

	// Adjust for the 5-hour difference between EST and UTC
	marketOpenEpoch -= 5 * 3600;

	// Convert the epoch to a string
	std::tm marketOpenTmStruct;
	std::tm* marketOpenTm = &marketOpenTmStruct;
	err = gmtime_s(marketOpenTm, &marketOpenEpoch);
	if (err != 0) {
		std::cerr << "Error converting result with gmtime_s." << std::endl;
	}
	std::ostringstream oss;
	oss << std::put_time(marketOpenTm, "%Y-%m-%d %H:%M:%S");

	return oss.str();
}
