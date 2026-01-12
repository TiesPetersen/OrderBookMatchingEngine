#include <chrono>
#include <iostream>
#include <vector>

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

int main() {
    OrderBook orderBook;
    const int num_orders = 100000;

    // 1. Pre-allocate memory and generate orders
    // We do this OUTSIDE the timer so we measure the Engine, not memory allocation.
    std::vector<Order> orders;
    orders.reserve(num_orders);

    for (int i = 0; i < num_orders; ++i) {
        // Alternate BUY and SELL to simulate trading activity
        Side side = (i % 2 == 0) ? BUY : SELL;
        Price price = 100.0;  // Same price to force collision/matching
        orders.emplace_back(i, 0, side, LIMIT, price, 10);
    }

    std::cout << "Benchmarking " << num_orders << " orders..." << std::endl;

    // 2. Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    // 3. Execute Engine
    for (const auto& order : orders) {
        orderBook.PlaceOrder(order);
    }

    // 4. Stop Timer
    auto end = std::chrono::high_resolution_clock::now();

    // 5. Calculate Metrics
    std::chrono::duration<double> diff = end - start;
    double seconds = diff.count();

    std::cout << "Time: " << seconds << " seconds\n";
    std::cout << "Throughput: " << (num_orders / seconds) << " orders/second\n";

    return 0;
}