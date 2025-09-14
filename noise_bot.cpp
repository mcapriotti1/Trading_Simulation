#include "noise_bot.hpp"

NoiseBot::NoiseBot(int id_)
  : Bot(id_),
  rng(std::random_device{}() ^ (id_ << 10)),
  chanceDist(50.0, 15.0),
  maxAmountDist(3.0, 1.2), 
  adjustmentDist(-0.01, 0.01),
  hundredDist(0, 99)
{
  buyChance  = std::clamp(static_cast<int>(std::round(chanceDist(rng))), 10, 90);
  sellChance = std::clamp(static_cast<int>(std::round(chanceDist(rng))), 10, 90);
  maxBuy     = std::clamp(static_cast<int>(std::round(maxAmountDist(rng))), 1, 5);
  maxSell    = std::clamp(static_cast<int>(std::round(maxAmountDist(rng))), 1, 5);
}

TradeOrder NoiseBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, 1};

  int rBuy  = hundredDist(rng);
  int rSell = hundredDist(rng);
  double adjustment = adjustmentDist(rng);

  if (rBuy < buyChance && cash >= tick.ask) {
      order.type = TradeType::BUY;
      order.amount = std::min(maxBuy, static_cast<int>(cash / tick.ask));
      order.price = tick.ask - adjustment;
  }
  else if (holdings > 0 && rSell < sellChance) {
      order.type = TradeType::SELL;
      order.amount = std::min(maxSell, holdings);
      order.price = tick.bid + adjustment;
  }

  return order;
}