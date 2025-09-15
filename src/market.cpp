#include "market.hpp"
#include <cstdlib>

void updateMarket(MarketTick& tick) {
  // Drift of -0.1, 0, or 0.1
  double drift = ((rand() % 3) - 1) * 0.1;
  tick.lastPrice += drift;
  if (tick.bid < tick.lastPrice - 0.005) tick.bid += 0.001;
  if (tick.ask > tick.lastPrice + 0.005) tick.ask -= 0.001;
}