#pragma once
#include "bot.hpp"

struct NoiseBot : public Bot {
   int buyChance;
    int sellChance;
    int maxBuy;  
    int maxSell;

    std::default_random_engine rng;
    std::normal_distribution<double> chanceDist;
    std::normal_distribution<double> maxAmountDist;
    std::uniform_real_distribution<double> adjustmentDist;
    std::uniform_int_distribution<int> hundredDist;

    NoiseBot(int id_);
    TradeOrder decide(const MarketTick &tick) override;

};