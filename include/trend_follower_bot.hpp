#pragma once
#include "bot.hpp"
#include <deque>

struct TrendFollowerBot: public Bot {
  double buyBias;
  double sellBias;
  int memoryWindow;
  double jitter;
  int ttl;

  std::deque<double> priceHistory;

  std::default_random_engine rng;
  std::normal_distribution<double> biasDist;
  std::normal_distribution<double> jitterDist;
  std::normal_distribution<double> memoryDist;
  
  TrendFollowerBot(int id_);
  TradeOrder decide(const MarketTick &tick) override;
};