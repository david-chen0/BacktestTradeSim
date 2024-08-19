# BacktestTradeSim
Personal project for backtesting framework for my trading ideas. Still WIP

## Implementation
### Transaction Class
Represents a transaction.

Variables:
* security: The security that was traded
* cost: The cost to acquire this security
* date: The day this security was acquired(Can also think about swapping this for `timestamp` represented with an epoch number)
* securityType: The type of security that was traded(stock, option, future, bond, swap. Will likely limit to stock for now)
* direction: The direction of the security(long or short)
* strikePrice: Strike price(only if we are doing options)
* 

### Strategy Class
Represents a trading strategy

Ideas and logic moved to Strategy.hpp

### DataFetcher Class
Fetches the data from our data source

Variables:
* apiKey: This will likely either be one of the inputs of the program or be required to be hardcoded into a file. Make sure to add it to .gitignore if we do hardcode it

Methods:
* loadData: Loads the data for the input security, time(window), and securityType

### Main functionality
This will be the main file that will be called and route the utility calls

Variables:
* strategy: This will be the strategy that is used

Methods:
* chooseTimeframe: Allows the user to pick their timeframe to run the test over
* runBacktest: Runs the backtest itself
* loadData: This will likely be a helper method called in `runBacktest` to get the needed data and polish it
* applyStrategy: This will assign the strategy that we want to use. Can maybe be in the `init` rather than as a separate method

## Data
Find an open source Yahoo Finance C++ library. Choosing this because Yahoo finance libraries will give good historical data but not real-time data, which we don't need. Also since there wouldn't be any throttling.

## Graphing
C++ Backend: Processes data, exports results to JSON.
Web Front-End: Fetches JSON data, visualizes it using React framework and libraries like Chart.js.
Server: Serve static files or use a dynamic API to deliver data. Can likely skip this step since the user will be the server.
Deployment: Host your application on Github Pages.

## Testing
Make some unit tests for the methods

Some easier integ tests(ex: can pick and choose a strategy such as dollar cost averaging for integ test)
