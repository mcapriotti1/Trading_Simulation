#include "market_maker_bot.hpp"

MarketMakerBot::MarketMakerBot(int id_)
  : Bot(id_),
    rng(std::random_device{}() ^ (id_ << 12)),
    spreadDist(0.005, 0.01),
    orderSizeDist(3.0, 1.5),
    coinFlip(0, 1)
{
  spread = std::clamp(spreadDist(rng), 0.01, 0.15);
  orderSize = std::clamp(static_cast<int>(std::round(orderSizeDist(rng))), 1, 10);
}

TradeOrder MarketMakerBot::decide(const MarketTick &tick) {
  TradeOrder order{TradeType::BUY, 0, 0.0, id, 1};

    // --- BUY ORDER ---
    double buyPrice = tick.lastPrice - spread;
    if (cash >= buyPrice && coinFlip(rng) == 1) {
        int affordableAmount = static_cast<int>(cash / buyPrice);
        order.type = TradeType::BUY;
        order.amount = std::min(orderSize, affordableAmount);
        order.price = buyPrice;
    }
    else if (holdings > 0 && coinFlip(rng) == 0) {
        order.type = TradeType::SELL;
        order.amount = std::min(orderSize, holdings);
        order.price = tick.lastPrice + spread;
    }

    return order;
}