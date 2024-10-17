#include "../include/data/DataFetcher.hpp"
#include "../include/util/Security.hpp"
#include "../include/data/SecurityData.hpp"

#include <ctime>
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>

using json = nlohmann::json;

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
// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
//static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
//	((std::string*)userp)->append((char*)contents, size * nmemb);
//	return size * nmemb;
//}

// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
//// Callback to handle headers and extract cookies
//static size_t headerCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
//	std::string* cookies = static_cast<std::string*>(userdata);
//	std::string header(buffer, size * nitems);
//	if (header.find("Set-Cookie:") != std::string::npos) {
//		// Extract the cookie value
//		size_t start = header.find("Set-Cookie:") + 11;
//		size_t end = header.find(";", start);
//		if (end != std::string::npos) {
//			cookies->append(header.substr(start, end - start));
//			cookies->append("; ");
//		}
//	}
//	return nitems * size;
//}

// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
//void getCrumbAndCookie() {
//	CURL* curl = curl_easy_init();
//	if (!curl) {
//		std::cerr << "Failed to initialize cURL" << std::endl;
//		return;
//	}
//
//	FILE* crumbfile = nullptr;
//	errno_t err = fopen_s(&crumbfile, DataFetcher::crumbFile.c_str(), "wb");
//	if (err != 0 || !crumbfile) {
//		std::cerr << "Failed to open crumb file, error code: " << err << std::endl;
//		curl_easy_cleanup(curl);
//		return;
//	}
//	std::cerr << "Found crumb file" << std::endl;
//
//	//std::string url = "https://finance.yahoo.com/quote/AAPL/history?p=AAPL";
//	std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/AAPL";
//	//std::string url = "https://query1.finance.yahoo.com/v1/test/getcrumb";
//	//std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/AAPL?metrics=high?&interval=1d&range=5d";
//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, DataFetcher::cookieFile.c_str());
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, crumbfile);
//	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
//
//	// delete these after debugging
//	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36");
//	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//	std::cerr << "Starting cURL request to fetch cookie and crumb" << std::endl;
//	CURLcode res = curl_easy_perform(curl);
//	if (res != CURLE_OK) {
//		std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
//	} else {
//		long response_code;
//		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code); // Get HTTP response code
//		std::cout << "Response code: " << response_code << std::endl;
//		std::cout << "Data successfully written to file" << std::endl;
//	}
//
//	// Clean up
//	fclose(crumbfile);
//	curl_easy_cleanup(curl);
//}

// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
// Reads the crumb from the crumb file
//std::string getCrumbFromFile() {
//	std::ifstream crumbFileStream(DataFetcher::crumbFile.c_str());
//	std::string crumb;
//
//	if (crumbFileStream.is_open()) {
//		std::string line;
//		while (getline(crumbFileStream, line)) {
//			size_t pos = line.find("CrumbStore\":{\"crumb\":\"");
//			if (pos != std::string::npos) {
//				pos += 22;  // Offset to the start of the crumb
//				size_t endPos = line.find("\"", pos);
//				crumb = line.substr(pos, endPos - pos);
//				break;
//			}
//		}
//		crumbFileStream.close();
//	}
//	else {
//		std::cerr << "Failed to read crumb file" << std::endl;
//	}
//
//	return crumb;
//}

// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
// Use the cookie file in your main queries
//void setCookieFile(CURL* curl) {
//	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, DataFetcher::cookieFile.c_str());  // Use cookies from file
//}

std::string buildURL(
	const Security& security,
	const std::string& startDate,
	const std::string& endDate
) {
	return "https://query1.finance.yahoo.com/v8/finance/chart/" + security.getIdentifier() +
		"?period1=" + startDate + "&period2=" + endDate + "&interval=1d&events=history";
}

const std::string DataFetcher::getDataLocation(std::string identifier) {
	return DataFetcher::fetchedDataFolder + "/" + identifier + ".txt";
}

void DataFetcher::performRequest(
	const Security& security,
	const std::string& startDate,
	const std::string& endDate
) {
	//getCrumbAndCookie();

	CURL* curl;
	CURLcode res;

	curl = curl_easy_init();

	// NOT NEEDED: Yahoo API V8 doesn't require crumbs and cookies
	//setCookieFile(curl);
	//std::string crumb = getCrumbFromFile();

	std::string url = buildURL(security, startDate, endDate);

	if (!curl) {
		throw std::runtime_error("Failed to initiate curl");
	}

	// Creating the file if it doesn't exist, then opening it to be written to
	FILE* dataFile = nullptr;
	std::string dataFileName = getDataLocation(security.getIdentifier());
	errno_t err = fopen_s(&dataFile, dataFileName.c_str(), "wb");
	std::cout << "Opening file for cURL to write to at " << dataFileName << std::endl;
	if (err != 0 || !dataFile) {
		throw std::runtime_error("Failed to open data file");
		curl_easy_cleanup(curl);
		return;
	}

	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36");
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, dataFile);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);

	std::cout << "cURLing at URL: " << url << std::endl;
	res = curl_easy_perform(curl); // Performs the request
	if (res != CURLE_OK) {
		std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
	}
	std::cout << "Finished the cURL request for security " << security.getIdentifier() << std::endl;

	fclose(dataFile);
	curl_easy_cleanup(curl); // Frees the unused resources
}

bool stringNumberIsGreater(const std::string& num1, const std::string& num2) {
	// Convert strings to integers
	int number1 = std::stoi(num1);
	int number2 = std::stoi(num2);

	// Returns whether the first string is greater than second
	return number1 > number2;
}

double convertJsonToDouble(json input) {
	return std::stod((std::string) input);
}

std::map<Security, std::vector<SecurityData>> DataFetcher::fetchData(
	std::set<Security> securities,
	std::string& startDate,
	std::string& endDate
) {
	std::string actualStartDate = endDate; // Represents the actual start date of all queries
	std::string actualEndDate = startDate; // Represents the actual end date of all queries
	std::map<Security, std::vector<SecurityData>> result;
	for (const Security security : securities) {
		std::string dataFileName = getDataLocation(security.getIdentifier());
		performRequest(security, startDate, endDate);

		// Open the file
		std::cout << "Opening file " << dataFileName << std::endl;
		std::ifstream file(dataFileName);

		if (!file.is_open()) {
			throw std::runtime_error("Could not open file: " + dataFileName);
		}

		json jsonResponse;
		file >> jsonResponse;
		file.close();
		std::cout << "Finished reading from file " << dataFileName << std::endl;

		json dataFields = jsonResponse["chart"]["result"][0];
		json securityDataFields = dataFields["indicators"]["quote"][0];

		json timestamps = dataFields["timestamp"];
		json volumes = securityDataFields["volume"];
		json opens = securityDataFields["open"];
		json closes = securityDataFields["close"];
		json lows = securityDataFields["low"];
		json highs = securityDataFields["high"];

		// Converting all the data into SecurityData objects
		int numDates = dataFields["timestamp"].size();
		std::vector<SecurityData> securityDataVector;
		for (int i = 0; i < numDates; i++) {
			securityDataVector.push_back(SecurityData(
				std::to_string(timestamps[i].get<int>()),
				opens[i].get<double>(),
				closes[i].get<double>(),
				highs[i].get<double>(),
				lows[i].get<double>(),
				volumes[i].get<long>()
			));
			std::cout << "Finished iteration " << i << " of creating the SecurityData objects." << std::endl;
		}

		std::string firstTimestamp = std::to_string(timestamps[0].get<int>());
		// See the description of this method in the header for this
		if (stringNumberIsGreater(actualStartDate, firstTimestamp)) {
			actualStartDate = firstTimestamp;
		}
		std::string lastTimestamp = std::to_string(timestamps.back().get<int>());
		if (stringNumberIsGreater(lastTimestamp, actualEndDate)) {
			actualEndDate = lastTimestamp;
		}

		result.emplace(security, securityDataVector);
	}

	// Modifying the start and end date so the calling method would also have those values modified
	startDate = actualStartDate;
	endDate = actualEndDate;
	return result;
}


//std::map<Security, SecurityData> DataFetcher::fetchData(
//	std::set<Security> securities,
//	const std::string& startDate,
//	const std::string& endDate
//) {
//	std::map<Security, SecurityData> securityDataMap;
//	for (auto& security : securities) {
//		std::string data = performRequest(security, startDate, endDate);
//		SecurityData securityData = mapToSecurityData(data);
//		securityDataMap.insert({ security, securityData });
//	}
//
//	return securityDataMap;
//}

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
		std::getline(lineStream, volumeStr, ',');

		// Convert strings to appropriate types
		double open = std::stod(openStr);
		double high = std::stod(highStr);
		double low = std::stod(lowStr);
		double close = std::stod(closeStr);
		long volume = std::stol(volumeStr);

		// Create SecurityData object
		SecurityData securityData(date, open, close, high, low, volume);

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
