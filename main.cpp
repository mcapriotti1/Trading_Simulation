#include <iostream>
#include <random>
#include "simulation.hpp"

int main() {
    srand(time(0));
    std::mt19937 g(rand());

    int numTicks = 200;
    int totalBots = 250;
    float bid = 4.99, ask = 5.01, price = 5.00;
    std::string CSV_FILE = "portfolio_summary.csv";

    int numBots = totalBots / 5;
    int numTypes = 5;

    std::vector<Bot*> bots;
    std::vector<int> botTypes;
    createBots(numBots, bots, botTypes);

    MarketTick tick = {bid, ask, price, 10000};

    std::vector<std::vector<double>> botHistory(bots.size());
    std::vector<double> marketHistory;

    runSimulation(numTicks, tick, bots, botHistory, marketHistory, g);
    saveCSV(numTicks, numTypes, CSV_FILE, botTypes, bots, botHistory, marketHistory);

    cleanup(bots);
    return 0;
}