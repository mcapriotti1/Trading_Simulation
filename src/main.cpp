#include <iostream>
#include <random>
#include "simulation.hpp"

int main() {
  srand(time(0));

  int numTicks = 2000;
  int totalBots = 500;
  float bid = 4.99, ask = 5.01, price = 5.00;
  std::string CSV_FILE = "portfolioSummary.csv";

  int numBots = totalBots / 5;
  int numTypes = 5;

  std::vector<Bot*> bots;
  std::vector<int> botTypes;
  createBots(numBots, bots, botTypes);

  MarketTick tick = {bid, ask, price, 50000};

  std::vector<std::vector<double>> botHistory(bots.size());
  std::vector<double> marketHistory;

  runSimulation(numTicks, tick, bots, botHistory, marketHistory);
  std::cout << "Simulation Completed" << std::endl;
  saveCSV(numTicks, numTypes, CSV_FILE, botTypes, bots, botHistory, marketHistory);
  std::cout << "Data Saved to CSV File" << std::endl;

  cleanup(bots);
  return 0;
}