#include "bot.hpp"

void Bot::applyTrade(const TradeOrder &order, int executed, double price) {
    if (order.type == TradeType::BUY) {
        avgPurchasePrice = (avgPurchasePrice * holdings + executed * price) / (holdings + executed);
        holdings += executed;
        cash -= executed * price;
    } else {
        holdings -= executed;
        cash += executed * price;
    }
}

double Bot::portfolioValue(const MarketTick &tick) const {
    return cash + holdings * tick.lastPrice;
}
