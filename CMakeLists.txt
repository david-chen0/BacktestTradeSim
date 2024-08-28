cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BacktestTradeSim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find the required packages
find_package(CURL REQUIRED)

# Add include directory
include_directories(include)
include_directories(${CURL_INCLUDE_DIRS})

# Add executables
add_executable(
	BacktestTradeSim 
	src/Main.cpp
	src/Strategy.cpp
	src/Transaction.cpp
	src/data/DataFetcher.cpp
)

# Add tests
enable_testing()
add_executable(
	runTests
	tests/Temp.cpp
)
# add_test(NAME Test COMMAND runTests)

# Link required libraries
target_link_libraries(DataFetcher ${CURL_LIBRARIES})
