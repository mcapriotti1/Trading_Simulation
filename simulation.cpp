#include "bot.hpp"
#include "value_investor_bot.hpp"
#include <iostream>
#include <fstream>
#include <numeric>

void createBots(int numBots, std:: vector<Bot*>& bots, std::vector<int>& botTypes) {
    for (int i = 0; i < numBots; i++) { bots.push_back(new ValueInvestorBot(i)); botTypes.push_back(0); }
    // for (int i = 0; i < numBots; i++) { bots.push_back(new TrendFollowerBot(numBots+i)); botTypes.push_back(1); }
    // for (int i = 0; i < numBots; i++) { bots.push_back(new StopLossBot(2*numBots+i)); botTypes.push_back(2); }
    // for (int i = 0; i < numBots; i++) { bots.push_back(new MarketMakerBot(3*numBots+i)); botTypes.push_back(3); }
    // for (int i = 0; i < numBots; i++) { bots.push_back(new NoiseBot(4*numBots+i)); botTypes.push_back(4); }
};


std::vector<TradeOrder> collectOrders(const std::vector<Bot*>& bots, const MarketTick& tick) {
    std::vector<TradeOrder> orders;
    for (auto bot : bots) {
        TradeOrder order = bot->decide(tick);
        if (order.amount > 0) {
            orders.push_back(order);
        }
    }
    return orders;
}

void processOrders(MarketTick& tick,
                   std::vector<TradeOrder>& orders,
                   std::vector<Bot*>& bots, float& pulse) {
    std::cout << "--- Processing Orders ---\n";
    std::cout << "Initial orders count: " << orders.size() << "\n";

    // --- Decrement TTL and remove expired ---
    orders.erase(
        std::remove_if(orders.begin(), orders.end(),
                       [](TradeOrder& o) { return --o.ttl < 0; }),
        orders.end()
    );

    std::cout << "Orders after TTL check: " << orders.size() << "\n";

    // --- Split into buys and sells ---
    std::vector<TradeOrder> buyOrders;
    std::vector<TradeOrder> sellOrders;

    for (auto& order : orders) {
        if (order.type == TradeType::BUY) buyOrders.push_back(order);
        else if (order.type == TradeType::SELL) sellOrders.push_back(order);
    }

    std::cout << "Buy orders: " << buyOrders.size() << ", Sell orders: " << sellOrders.size() << "\n";

    // --- Sort orders ---
    std::sort(buyOrders.begin(), buyOrders.end(),
              [](const TradeOrder& a, const TradeOrder& b) { return a.price > b.price; });
    std::sort(sellOrders.begin(), sellOrders.end(),
              [](const TradeOrder& a, const TradeOrder& b) { return a.price < b.price; });


    std::cout << "--- Sorted Buy Orders ---\n";
    for (const auto& order : buyOrders) {
        std::cout << "Bot " << order.botId 
                << " Buy " << order.amount 
                << " @ " << order.price << "\n";
    }

    std::cout << "--- Sorted Sell Orders ---\n";
    for (const auto& order : sellOrders) {
        std::cout << "Bot " << order.botId 
                << " Sell " << order.amount 
                << " @ " << order.price << "\n";
    }

    size_t buyIdx = 0, sellIdx = 0;

    // --- Match orders ---
    while (buyIdx < buyOrders.size() && sellIdx < sellOrders.size()) {
        TradeOrder &buy = buyOrders[buyIdx];
        TradeOrder &sell = sellOrders[sellIdx];

        std::cout << "Attempting match: Buy(bot " << buy.botId << ", " << buy.amount << "@" << buy.price << ") "
                  << "Sell(bot " << sell.botId << ", " << sell.amount << "@" << sell.price << ")\n";

        if (buy.price >= sell.price) {
            int tradeAmount = std::min(buy.amount, sell.amount);

            // Execute trade (affects market volume)
            int preVolume = tick.volume;
            TradeOrder tradeBuy = buy;
            tradeBuy.amount = tradeAmount;
            executeTrade(tick, tradeBuy);
            int executed = std::abs(preVolume - tick.volume);
            float executedPrice = tradeBuy.price;

            // pulse += executed * executedPrice;
            double alpha = (double)executed / std::max(1L, tick.volume); // fraction of current market volume
            alpha = std::min(alpha, 0.05); // cap max price change per trade to 5%
            tick.lastPrice = tick.lastPrice * (1 - alpha) + executedPrice * alpha;

            std::cout << "Trade executed: " << executed << " shares at " << executedPrice << "\n";

            // Update bots
            bots[buy.botId]->applyTrade(tradeBuy, executed, executedPrice);
            bots[sell.botId]->applyTrade(sell, executed, executedPrice);

            // Reduce remaining amounts
            buy.amount -= tradeAmount;
            sell.amount -= tradeAmount;

            std::cout << "Remaining Buy amount: " << buy.amount << ", Remaining Sell amount: " << sell.amount << "\n";

            // Move to next order if fully filled
            if (buy.amount <= 0) buyIdx++;
            if (sell.amount <= 0) sellIdx++;
        } else {
            std::cout << "No more compatible orders (Buy price < Sell price)\n";
            break;
        }
    }

    // Remaining unmatched orders
    if (buyIdx < buyOrders.size() || sellIdx < sellOrders.size()) {
        std::cout << "Unmatched orders remain: "
                  << "BuyIdx=" << buyIdx << "/" << buyOrders.size()
                  << ", SellIdx=" << sellIdx << "/" << sellOrders.size() << "\n";
    }
    std::cout << "--- End Processing ---\n";
}

void updateMarket(MarketTick& tick, int pulse) {

    double drift = ((rand() % 3) - 1) * 0.1;
    tick.lastPrice += drift;
    if (tick.bid < tick.lastPrice - 0.005) tick.bid += 0.001;
    if (tick.ask > tick.lastPrice + 0.005) tick.ask -= 0.001;
}

void recordHistory(const std::vector<Bot*>& bots,
                   std::vector<std::vector<double>>& botHistory,
                   std::vector<double>& marketHistory,
                   const MarketTick& tick) {
    marketHistory.push_back(tick.lastPrice);

    for (int i = 0; i < bots.size(); i++) {
        botHistory[i].push_back(bots[i]->portfolioValue(tick));
    }
}

void runSimulation(int numTicks, MarketTick& tick,
                   std::vector<Bot*>& bots,
                   std::vector<std::vector<double>>& botHistory,
                   std::vector<double>& marketHistory,
                   std::mt19937& g) {
    for (int t = 0; t < numTicks; t++) {
        std::cout << "Tick " << t << ", LastPrice: " << tick.lastPrice
                  << ", Volume: " << tick.volume << "\n";

        auto orders = collectOrders(bots, tick);
        float pulse = 0;
        processOrders(tick, orders, bots, pulse);
        updateMarket(tick, pulse);
        recordHistory(bots, botHistory, marketHistory, tick);
    }
}


void saveCSV(int numTicks, int numTypes, std::string CSV_File, std::vector<int>& botTypes, std::vector<Bot*>& bots, std::vector<std::vector<double>>& botHistory, std::vector<double>& marketHistory) {
    std::ofstream summary(CSV_File);
    summary << "tick,lastPrice";
    std::vector<std::string> typeNames = {"ValueInvestor","TrendFollower","StopLoss","MarketMaker","Noise"};
    for (auto &name : typeNames) summary << ",best_" << name << ",worst_" << name << ",avg_" << name;
    summary << "\n";

    for (int t = 0; t < numTicks; t++) {
        summary << t << "," << marketHistory[t]; // use stored market price

        for (int type = 0; type < numTypes; type++) {
            std::vector<double> values;
            for (int i = 0; i < bots.size(); i++)
                if (botTypes[i] == type)
                    values.push_back(botHistory[i][t]);

            double best = *std::max_element(values.begin(), values.end());
            double worst = *std::min_element(values.begin(), values.end());
            double avg = std::accumulate(values.begin(), values.end(), 0.0) / values.size();

            summary << "," << best << "," << worst << "," << avg;
        }
        summary << "\n";
    }

    summary.close();
}

void cleanup(std::vector<Bot*>& bots) {
    for (auto bot : bots) delete bot;
}