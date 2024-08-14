# BacktestTradeSim
Personal project for backtesting framework for my trading ideas. Still WIP

## Plan
### Implementation
#### Transaction Class
Represents a transaction.

Variables:
* security: The security that was traded
* cost: The cost to acquire this security
* date: The day this security was acquired(Can also think about swapping this for `timestamp` represented with an epoch number)
* securityType: The type of security that was traded(stock, option, future, bond, swap. Will likely limit to stock for now)
* direction: The direction of the security(long or short)
* strikePrice: Strike price(only if we are doing options)
* 

#### Strategy Class
Represents a trading strategy

Variables:
* securities: A set of all securities that were touched, even if their positions were closed
* position: Some way to track all positions acquired and closed(ex: hashmap from security name to list of transactions ordered by time the transaction was made)
* balance: The total net worth of the account(might not need this variable, since I'm thinking instead we have a method that calculates balance at a given timestamp)
* liquidBalance: The liquid cash that's available

Methods:
* processData: Takes in a data point and outputs an action(long, short, etc). Note that this will likely need to be extended to process multiple data points(ex: from different stocks) to account for some strategies(ex: pairs trading)
* currentPosition: Outputs current position(does not include closed positions)
* listTransactions: Lists all the transactions we've taken for the security(s) provided

### Data
Find an open source Yahoo Finance C++ library. Choosing this because Yahoo finance libraries will give good historical data but not real-time data, which we don't need. Also since there wouldn't be any throttling.

### Graphing
C++ Backend: Processes data, exports results to JSON.
Web Front-End: Fetches JSON data, visualizes it using React framework and libraries like Chart.js.
Server: Serve static files or use a dynamic API to deliver data. Can likely skip this step since the user will be the server.
Deployment: Host your application on Github Pages.

### Testing
Make some unit tests for the methods

Some easier integ tests(ex: can pick and choose a strategy such as dollar cost averaging for integ test)
