cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BacktestTradeSim VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Temporary set of CMake directory
set(CURL_LIBRARY "D:/curl/lib/libcurl.a")
set(CURL_INCLUDE_DIR "D:/curl/include")

# Specify the output directory for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find the required packages
find_package(CURL REQUIRED)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CURL_INCLUDE_DIRS})

# Recursively find all .cpp files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Add executables
add_executable(
	BacktestTradeSim 
	${SOURCE_FILES}
)

# Add tests
#enable_testing()
#add_executable(
#	BacktestTradeSimTests
#	tests/Temp.cpp
#)
# add_test(NAME Test COMMAND runTests)

# Link required libraries
target_link_libraries(BacktestTradeSim ${CURL_LIBRARIES})
