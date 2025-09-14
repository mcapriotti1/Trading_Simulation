#include "stop_loss_bot.hpp"

StopLossBot::StopLossBot(int id_)
  : Bot(id_),
    rng(std::random_device{}() ^ (id_ << 16)),
    stopLossDist(0.12, 0.05),
    buyFractionDist(0.4, 0.2),
    buyChanceDist(30.0, 20.0),
    sellChunkDist(4.0, 1.5),
    hundredDist(0, 99)
{
  stopLossPercent = std::clamp(stopLossDist(rng), 0.03, 0.3); 
  buyFraction = std::clamp(buyFractionDist(rng), 0.05, 0.9);
  buyChancePercent = std::clamp(static_cast<int>(std::round(buyChanceDist(rng))), 5, 90);
  sellChunks = std::clamp(static_cast<int>(std::round(sellChunkDist(rng))), 1, 8);

  highestPriceSeen = 0.0;
}


TradeOrder StopLossBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, 1};

  if (holdings > 0) {
      highestPriceSeen = std::max(highestPriceSeen, tick.lastPrice);
  }

  if (holdings > 0 && tick.lastPrice < highestPriceSeen * (1.0 - stopLossPercent)) {
      order.type = TradeType::SELL;
      order.amount = std::max(1, holdings / sellChunks);
      order.price = tick.bid;
      return order;
  }

  if (cash >= tick.ask) {
      int roll = hundredDist(rng);
      if (roll < buyChancePercent) {
          int maxAffordable = static_cast<int>(cash / tick.ask);
          int buyAmount = std::max(1, int(maxAffordable * buyFraction));
          order.type = TradeType::BUY;
          order.amount = buyAmount;
          order.price = tick.ask;

          highestPriceSeen = tick.lastPrice;
          return order;
      }
  }

  return order;
}
