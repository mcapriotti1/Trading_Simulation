#include "trade.hpp"
#include <algorithm>
#include <cstdlib>

void executeTrade(MarketTick &tick, TradeOrder &order) {
    if (order.amount <= 0) return;

    double tradeImpact = static_cast<double>(order.amount) / std::max(1L, tick.volume + order.amount);
    tradeImpact = std::min(tradeImpact, 0.1);

    double direction = (order.type == TradeType::BUY) ? 1.0 : -1.0;

    tick.lastPrice += direction * tradeImpact * (order.price - tick.lastPrice);

    double midPrice = (tick.bid + tick.ask) / 2.0;
    tick.lastPrice = tick.lastPrice * 0.95 + midPrice * 0.05;

    double noise = ((rand() % 21) - 10) / 1000.0;
    tick.lastPrice *= 1.0 + noise;

    tick.volume += order.amount;

    double spread = 0.01 + ((rand() % 3) / 100.0);
    tick.bid = tick.lastPrice - spread;
    tick.ask = tick.lastPrice + spread;
}
