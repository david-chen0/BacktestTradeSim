cmake_minimum_required(VERSION 3.10)
project(BacktestTradeSim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

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
