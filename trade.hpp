#pragma once
#include "market.hpp"

enum class TradeType { BUY, SELL };

struct TradeOrder {
    TradeType type;
    int amount;
    double price;
    int botId;
    int ttl; // Time To Live
};

void executeTrade(MarketTick &tick, TradeOrder &order);
