#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

const int kMinPrice = 800'00;
const int kMaxPrice = 1200'00;
const int kStartPrice = 1000'00;

int main() {
    OrderBook orderBook;
    const int num_orders = 3000;

    // TODO: warm-up + populate book with initial orders

    random_device rd;
    default_random_engine generator(rd());

    // Price movement distribution
    normal_distribution<double> price_difference(0.0, 0.5);
    double current_mid_price = kStartPrice;

    // Type distribution: 0 for MARKET, 1 for LIMIT, 2 for CANCEL
    discrete_distribution<int> type_distribution({10, 70, 20});

    // Side distribution: 0 for BUY, 1 for SELL
    uniform_int_distribution<int> side_distribution(0, 1);

    // Price distribution for limit orders
    // normal_distribution<double> price_offset_distribution(0.0, 10.0);
    geometric_distribution<int> price_offset_distribution(0.2);

    // Volume distribution
    geometric_distribution<int> volume_distribution(0.1);

    // Pre-allocate memory
    vector<Order> orders;
    orders.reserve(num_orders);

    // Price movement array
    vector<double> price_movements;
    price_movements.reserve(num_orders);

    // Generate Orders
    for (int i = 0; i < num_orders; ++i) {
        // Simulate mid-price movement
        current_mid_price += price_difference(generator);
        current_mid_price = max<double>(current_mid_price, kMinPrice);
        current_mid_price = min<double>(current_mid_price, kMaxPrice);

        // Save price movement
        price_movements.push_back(current_mid_price / 100.0);

        // Order type
        auto type = static_cast<OrderType>(type_distribution(generator));

        // CANCEL order
        if (type == CANCEL) {
            if (!orders.empty()) {
                // Randomly select an existing order to cancel
                uniform_int_distribution<size_t> index_distribution(
                    0, orders.size() - 1);
                size_t index_to_cancel = index_distribution(generator);
                OrderID order_id_to_cancel =
                    orders[index_to_cancel].getOrderId();

                orders.emplace_back(i, BUY, CANCEL, 0, 0, order_id_to_cancel);
            }
            continue;
        }

        // MARKET or LIMIT order

        // Order side
        Side side = side_distribution(generator) == 0 ? BUY : SELL;

        // Order price
        Price price;
        if (type == LIMIT) {
            int price_offset = price_offset_distribution(generator);

            if (side == BUY) {
                price = static_cast<Price>(current_mid_price) - price_offset;
            } else {
                price = static_cast<Price>(current_mid_price) + price_offset;
            }

            price = max<Price>(price, kMinPrice);
            price = min<Price>(price, kMaxPrice);
        } else {
            price = 0;  // Price is not used for MARKET orders
        }

        // Order volume
        Volume volume = volume_distribution(generator);

        orders.emplace_back(i, side, type, price, volume);
    }

    // write price movement (+ limit order price if LIMIT) to text file
    ofstream outfile("price_movement.txt");
    for (size_t i = 0; i < orders.size(); ++i) {
        const auto& order = orders[i];
        outfile << price_movements[i] << "\n";
        if (order.getOrderType() == LIMIT) {
            outfile << price_movements[i] << " " << order.getPrice() / 100.0
                    << " " << static_cast<int>(order.getSide()) << "\n";
        }
    }
    outfile.close();

    // cout << "Benchmarking " << num_orders << " orders..." << "\n";

    // // Start Timer
    // auto start = chrono::high_resolution_clock::now();

    // // Execute Engine
    // for (const auto& order : orders) {
    //     orderBook.PlaceOrder(order);
    // }

    // // Stop Timer
    // auto end = chrono::high_resolution_clock::now();

    // // Calculate Metrics
    // chrono::duration<double> diff = end - start;
    // double seconds = diff.count();

    // cout << "Time: " << seconds << " seconds" << "\n";
    // cout << "Throughput: " << (num_orders / seconds) << " orders/second"
    //      << "\n";
    // return 0;
}