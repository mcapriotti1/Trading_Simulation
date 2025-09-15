# Trading_Simulation
---
A C++ single ticker market simulation with 5 variations of trading bots. Every tick a bot can submit a single order, which is placed and executed (if possible) into a price sorted order book. Every bot's portfolio is tracked throughout the simulation, and the best, worst, and average for each type of bot is saved and plotted.

## About the Project

**Tech Used:** C++, Python, Matplotlib, CSV

### The Simulation
The simulation starts with 500 bots (100 of each type) and an initial market state of last price 5.00, bid 5.01, and ask 4.99. At each tick, every bot can place a buy or sell order, or choose to do nothing. Each order specifies a price, amount, and time-to-live (TTL).

After all bots submit orders, they are collected into an order book sorted by price. Individual units are then matched by pairing the highest bid with the lowest ask, executing trades that adjust the last price based on the order price. Once all possible trades are executed, the market undergoes a small random drift, finalizing the last price for that tick.

The simulation records the last price and the portfolio value (cash + holdings × last price) for every bot at each tick. This process repeats until the tick limit (2,000 ticks) is reached. The parameters (number of bots, tick limit, etc.) were chosen via trial and error to produce a market that is dynamic yet relatively stable.

### The Bots

The 5 trading bots implemented were a Value Investor, Stop Loss, Trend Follower, Market Maker, and Noise. 
**1. Value Investor:** Buys when the price is below their target and sells when above, using a randomized profit margin.


### Trading Bots
**1. Value Investor:** a

**2. Trend Follower:** b

**3. Stop Loss:** c

**4. Market Maker:** d

**5. Noise:** e

Buys when the price is below their target and sells when above, using a randomized profit margin.

Trend Follower: Observes a memory window of past ticks to follow upward or downward trends, adjusting buy/sell decisions accordingly.

Stop Loss: Tracks the highest price seen and sells if the price drops below a threshold.

Market Maker: Places buy/sell orders around the current market price, adjusting spread and order size randomly.

Noise Bot: Trades randomly with stochastic buy/sell chances and varying amounts to simulate market noise.

Each bot generates orders with Time-To-Live (TTL) and amounts, and the market maintains an order book sorted by highest buy and lowest sell prices. The simulation records:

Ticker history (last price at each tick)

Portfolio performance for each bot type (best, worst, and average)

Results are exported as CSV files, which can be visualized using Python and Matplotlib for portfolio comparisons and market evolution.

Demos

Portfolio Overview
Displays best, worst, and average portfolio value over time for all bot types.

Market Price Evolution
Shows the ticker price trajectory over the simulation period.

Bot-Specific Behavior
Plots highlighting how different bot types react to market conditions (trend-following, stop-loss triggers, noise trading, etc.)

(You can insert screenshots here if desired.)

Running Instructions

Clone the Repository

