cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BacktestTradeSim VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

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

# Temporary set of CMake directory
#set(CURL_LIBRARY "D:/curl/lib/libcurl.dll.a")
#set(CURL_INCLUDE_DIR "D:/curl/include")

# Find and link CURL
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(BacktestTradeSim ${CURL_LIBRARIES})

# Add nlohmann::json
include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2 # Check for the latest stable release
)

FetchContent_MakeAvailable(json)

# Link nlohmann::json to the project
target_link_libraries(BacktestTradeSim nlohmann_json)
