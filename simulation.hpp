#pragma once
#include "bot.hpp"
#include <vector>
#include <random>

void createBots(int numBots, std::vector<Bot*>& bots, std::vector<int>& botTypes);
std::vector<TradeOrder> collectOrders(const std::vector<Bot*>& bots, const MarketTick& tick);
void processOrders(MarketTick& tick, std::vector<TradeOrder>& orders, std::vector<Bot*>& bots, float& pulse);
void recordHistory(const std::vector<Bot*>& bots, std::vector<std::vector<double>>& botHistory, std::vector<double>& marketHistory, const MarketTick& tick);
void runSimulation(int numTicks, MarketTick& tick, std::vector<Bot*>& bots, std::vector<std::vector<double>>& botHistory, std::vector<double>& marketHistory);
void saveCSV(int numTicks, int numTypes, std::string& CSV_File, std::vector<int>& botTypes, std::vector<Bot*>& bots, std::vector<std::vector<double>>& botHistory, std::vector<double>& marketHistory);
void cleanup(std::vector<Bot*>& bots);
