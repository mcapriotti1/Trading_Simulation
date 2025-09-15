# Trading_Simulation

A C++ single ticker market simulation with 5 variations of trading bots. Every tick a bot can submit a single order, which is placed and executed (if possible) into a price sorted order book. Every bot's portfolio is tracked throughout the simulation, and the best, worst, and average for each type of bot is saved and plotted.

---

## About the Project

**Tech Used:** C++, Python, Matplotlib, CSV

### The Simulation
The simulation starts with 500 bots (100 of each type) and an initial market state of last price 5.00, bid 5.01, and ask 4.99. At each tick, every bot can place a buy or sell order, or choose to do nothing. Each order specifies a price, amount, and time-to-live (TTL).

After all bots submit orders, they are collected into an order book sorted by price. Individual units are then matched by pairing the highest bid with the lowest ask, executing trades that adjust the last price based on the order price. Once all possible trades are executed, the market undergoes a small random drift, finalizing the last price for that tick.

The simulation records the last price and the portfolio value (cash + holdings × last price) for every bot at each tick. This process repeats until the tick limit (2,000 ticks) is reached. The parameters (number of bots, tick limit, etc.) were chosen via trial and error to produce a market that is dynamic yet relatively stable. The ticker price as well as  best, worst, and average of each type of bot are calculated from the simulation records which are exported as a CSV file visualized with Matplotlib.

### Trading Bots
**1. Value Investor:** Buys when the price is below their target and sells when above, using a randomized profit margin.

**2. Trend Follower:** Observes a randomized memory window of past ticks to follow upward or downward trends, adjusting buy/sell decisions accordingly.

**3. Stop Loss:** Tracks the highest price seen and sells if the price drops below a randomized threshold.

**4. Market Maker:** Places buy/sell orders around the current market price, adjusting spread and order size randomly.

**5. Noise:** Trades randomly with stochastic buy/sell chances and varying amounts to simulate market noise.

## Demos

### Overview
- Displays best and average portfolio value over time for all bot types and ticker price.
<div style="text-align: center">
  <img src="visual/simulations/simulation1/bot_comparison.pdf" 
     alt="Demo Screenshot" 
     style="display: block; margin: 0 auto;">
</div>
![Overview](visual/simulations/simulation1/bot_comparison.pdf)

### Bot-Specific
- Plots different bot types with best, worst, and average portfolio.
![Bot-Specific](visual/simulations/simulation1/bots.pdf)

Running Instructions

Clone the Repository

