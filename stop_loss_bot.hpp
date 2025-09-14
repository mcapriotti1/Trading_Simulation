#pragma once
#include "bot.hpp"

struct StopLossBot : public Bot {
  double stopLossPercent;
  int sellChunks;
  int buyChancePercent;
  double buyFraction;
  double highestPriceSeen;

  std::default_random_engine rng;
  std::normal_distribution<double> stopLossDist;
  std::normal_distribution<double> buyFractionDist;
  std::normal_distribution<double> buyChanceDist;
  std::normal_distribution<double> sellChunkDist;
  std::uniform_int_distribution<int> hundredDist;

  StopLossBot(int id_);
  TradeOrder decide(const MarketTick &tick) override;

};
