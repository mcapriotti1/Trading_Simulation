#pragma once
#include "trade.hpp"

struct Bot {
    double cash = 500.0;
    int holdings = 5;
    double avgPurchasePrice = 5.0;
    int id;

    Bot(int id_) : id(id_) {}
    virtual ~Bot() {}

    virtual TradeOrder decide(const MarketTick &tick) = 0;

    virtual void applyTrade(const TradeOrder &order, int executed, double price);

    double portfolioValue(const MarketTick &tick) const;
};
