#include "trend_follower_bot.hpp"


TrendFollowerBot::TrendFollowerBot(int id_) 
  : Bot(id_),
    rng(std::random_device{}() ^ (id_ << 16)),
    biasDist(0.2, 0.1),
    jitterDist(10, 10),
    memoryDist(10, 8)
{
  buyBias = std::clamp(biasDist(rng), 0.2, 0.95);
  sellBias = std::clamp(biasDist(rng), 0.2, 0.95);
  memoryWindow = std::clamp(static_cast<int>(std::round(memoryDist(rng))), 3, 20);
  jitter = jitterDist(rng);

}

TradeOrder TrendFollowerBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, getRandomTTL()};
    
  // --- Update memory ---
  priceHistory.push_back(tick.lastPrice);
  if ((int)priceHistory.size() > memoryWindow)
      priceHistory.pop_front();

  // --- Evaluate price trend ---
  int rises = 0, falls = 0;
  for (size_t i = 1; i < priceHistory.size(); ++i) {
      if (priceHistory[i] > priceHistory[i-1]) rises++;
      else if (priceHistory[i] < priceHistory[i-1]) falls++;
  }

  // --- Trading logic based on observed trend ---
  if (rises >= static_cast<int>(memoryWindow * buyBias) && cash >= tick.ask) {
      int maxAffordable = static_cast<int>(cash / tick.ask);
      order.type = TradeType::BUY;
      order.amount = std::max(1, int(maxAffordable / (2.0 + jitter)));
      order.price = tick.ask;
  } 
  else if (falls >= static_cast<int>(memoryWindow * sellBias) && holdings > 0) {
      order.type = TradeType::SELL;
      order.amount = std::max(1, int(holdings / (2.0 + jitter)));
      order.price = tick.bid;
  }

  return order;
}
