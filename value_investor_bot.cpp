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
  profitMargin = std::clamp(marginDist(rng), 0.01, 0.3);
}

TradeOrder ValueInvestorBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, getRandomTTL()};
    
  double sellScore = (holdings > 0) ? (tick.lastPrice - avgPurchasePrice) / avgPurchasePrice : -1.0;
  double buyScore = (cash >= tick.ask) ? (avgPurchasePrice - tick.lastPrice) / avgPurchasePrice : -1.0;
  double adjustment = adjustmentDist(rng);

  if (buyScore > profitMargin && buyScore >= sellScore) {
      int maxAffordable = static_cast<int>(cash / tick.ask);
      int buyAmount = std::max(1, int(maxAffordable * buyScore));
      order.type = TradeType::BUY;
      order.amount = buyAmount;
      order.price = tick.ask + adjustment;
  } else if (sellScore > profitMargin) {
      int sellAmount = std::max(1, int(holdings * sellScore));
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
