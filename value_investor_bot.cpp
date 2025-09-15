#include "value_investor_bot.hpp"

ValueInvestorBot::ValueInvestorBot(int id_) 
  : Bot(id_),
    priceMean(5.0), priceStdDev(1.5),
    marginMean(0.05), marginStdDev(0.04),
    priceDist(priceMean, priceStdDev),
    marginDist(marginMean, marginStdDev),
    adjustmentDist(-0.01, 0.01),
    rng(std::random_device{}())
{
  avgPurchasePrice = std::max(0.5, priceDist(rng));
  // 1 - 10% Profit Margin
  profitMargin = std::clamp(marginDist(rng), 0.01, 0.1);
}

TradeOrder ValueInvestorBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, getRandomTTL()};
    
  double sellScore = (holdings > 0) ? (tick.lastPrice - avgPurchasePrice) / avgPurchasePrice : -1.0;
  double buyScore = (cash >= tick.ask) ? (avgPurchasePrice - tick.lastPrice) / avgPurchasePrice : -1.0;
  double adjustment = adjustmentDist(rng);
  int maxAffordable = static_cast<int>(cash / tick.ask);

  if (buyScore > profitMargin && buyScore >= sellScore) {
      int buyAmount = std::min(maxAffordable, int(maxAffordable / 2));
      order.type = TradeType::BUY;
      order.amount = buyAmount;
      order.price = tick.ask + adjustment;
  } else if (sellScore > profitMargin) {
      int sellAmount = std::min(maxAffordable, int(holdings / 2));
      order.type = TradeType::SELL;
      order.amount = sellAmount;
      order.price = tick.bid + adjustment;
  }
  return order;
}

void ValueInvestorBot::applyTrade(const TradeOrder &order, int executed, double price) {
  Bot::applyTrade(order, executed, price);
    
  if (order.type == TradeType::BUY && executed > 0) {
      avgPurchasePrice = (avgPurchasePrice * holdings + executed * price) / (holdings + executed);
  }
}
