#include "noise_bot.hpp"

NoiseBot::NoiseBot(int id_)
  : Bot(id_),
  rng(std::random_device{}() ^ (id_ << 10)),
  chanceDist(50.0, 50.0),
  maxAmountDist(20, 19), 
  adjustmentDist(-1, 1),
  hundredDist(0, 99)
{
  buyChance  = std::clamp(static_cast<int>(std::round(chanceDist(rng))), 10, 90);
  sellChance = std::clamp(static_cast<int>(std::round(chanceDist(rng))), 10, 90);
  maxBuy     = std::clamp(static_cast<int>(std::round(maxAmountDist(rng))), 1, 40);
  maxSell    = std::clamp(static_cast<int>(std::round(maxAmountDist(rng))), 1, 40);
}

TradeOrder NoiseBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, getRandomTTL()};

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