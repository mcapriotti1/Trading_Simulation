#pragma once
#include "bot.hpp"

struct MarketMakerBot: public Bot {
  double spread; 
  int orderSize;

  std::default_random_engine rng;
  std::normal_distribution<double> spreadDist;
  std::normal_distribution<double> orderSizeDist;
  std::uniform_int_distribution<int> coinFlip;

  MarketMakerBot(int id_);
  TradeOrder decide(const MarketTick &tick) override;
};