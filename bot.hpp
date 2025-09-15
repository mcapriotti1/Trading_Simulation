#pragma once
#include "trade.hpp"
#include <random>
#include <algorithm>
#include <cmath>

struct Bot {
    double cash = 500.0;
    int holdings = 5;
    double avgPurchasePrice = 5.0;
    int id;

    std::mt19937 rng;
    std::uniform_int_distribution<int> ttlDist;

    Bot(int id_) : 
                    id(id_),
                    rng(std::random_device{}() ^ (id_ << 16)),
                    ttlDist(1,3) {}
    virtual ~Bot() {}

    virtual TradeOrder decide(const MarketTick &tick) = 0;

    virtual void applyTrade(const TradeOrder &order, int executed, double price);

    double portfolioValue(const MarketTick &tick) const;

    int getRandomTTL() { return ttlDist(rng); }
};
