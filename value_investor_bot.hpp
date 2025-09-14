#pragma once
#include "bot.hpp"
#include <random>

struct ValueInvestorBot : public Bot {
    double profitMargin;
    double priceMean;
    double priceStdDev;
    double marginMean;
    double marginStdDev;

    std::default_random_engine rng;
    std::normal_distribution<double> priceDist;
    std::normal_distribution<double> marginDist;
    std::uniform_real_distribution<double> adjustmentDist;

    ValueInvestorBot(int id_);
    TradeOrder decide(const MarketTick &tick) override;
    void applyTrade(const TradeOrder &order, int executed, double price) override;
};
