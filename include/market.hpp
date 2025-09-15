#pragma once
#include <vector>

struct MarketTick {
    double bid;
    double ask;
    double lastPrice;
    long volume;
};

void updateMarket(MarketTick& tick);