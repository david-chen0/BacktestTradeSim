# BacktestTradeSim
Still very WIP, don't forget to update all of this

Current logic:
1. Strategy is initialized with a starting balance, start date, end date, and a set of securities to consider.

2. Data is cURLed from Yahoo finance's API. This will get a specific number of days of data first(number of days to retrieve can be overriden)
for all of the specified securities to consider. The purpose of this batching is to save memory locally and also reduce the number of cURL
requests we send.

Note that Yahoo finance API has a very big caveat of granularity,
as the data is only available by day rather than more granular such as by minute. This is perfectly fine for some longer term
strategies but fails for short term strategies or immediate strategies.

3. Overriding strategies must override the processData method, which takes in a map from Security to SecurityData and a string representing date.
The string represents the day to consider while the map maps from Security(all securities from the input set are included) to that security's
data for the day.

4. The strategy makes its decisions, places orders, and then the day is advanced until the end date is reached.


TODOs(in order from top to bottom):

TAKING A BREAK to work on different projects. The idea is to get a project that relies on this so that I would have an idea as to what I actually want from this backtesting framework

11/8
Track the portfolio's financial data over time(total value, total input amount, any future financial metrics)
currently have portfolio snapshots set up, where it'll take snapshots of the portfolio and store it in a map from date(string) to the snapshot
issue is that the snapshot currently holds a map from security to the number of shares. this won't tell us info about the security at that point in time though
can't just map from security to that security info at that time cuz why even map security atp, need to make it long term to support things like other fin metrics

11/10
Add graphing

11/12
Shouldn't keep track of security prices within Broker, figure out a different place to keep track of it.

11/14
Switch maps to unordered_maps for performance

FIGURE OUT HOW TO DO THIS AND MAKE A TIMELINE FOR IT
Make this more generic so that outside projects can call it too
Likely want to split processData into multiple types of calls, such as a getData call and a postTransaction call

Add financial metrics such as volatility(per stock and whole portfolio), covariance(per stock duo), sharpe ratio, etc
