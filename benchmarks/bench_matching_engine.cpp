#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

using namespace std;

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

const int kMinPrice = 800'00;
const int kMaxPrice = 1200'00;
const int kStartPrice = 1000'00;

int main() {
    const int num_orders = 30000000;

    // TODO: warm-up + populate book with initial orders

    // ----- Random Order Generation -----

    cout << "Generating " << num_orders << " random orders..." << "\n";

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
    geometric_distribution<int> price_offset_distribution(0.3);

    // Volume distribution
    geometric_distribution<int> volume_distribution(0.1);

    // Pre-allocate memory
    vector<Order> orders;
    orders.reserve(num_orders);

    // Generate Orders
    while (orders.size() < num_orders) {
        // Simulate mid-price movement
        current_mid_price += price_difference(generator);
        current_mid_price = max<double>(current_mid_price, kMinPrice);
        current_mid_price = min<double>(current_mid_price, kMaxPrice);

        // Order type
        auto type = static_cast<OrderType>(type_distribution(generator));

        // CANCEL order
        if (type == CANCEL) {
            continue;
            if (!orders.empty()) {
                // Randomly select an existing order to cancel
                uniform_int_distribution<size_t> index_distribution(
                    0, orders.size() - 1);
                size_t index_to_cancel = index_distribution(generator);
                OrderID order_id_to_cancel =
                    orders[index_to_cancel].getOrderId();

                // make sure we don't try to cancel an order that is already a CANCEL
                while (orders[index_to_cancel].getOrderType() == CANCEL) {
                    index_to_cancel = index_distribution(generator);
                    order_id_to_cancel = orders[index_to_cancel].getOrderId();
                }

                // Create CANCEL order
                orders.emplace_back(orders.size(), BUY, CANCEL, 0, 0,
                                    order_id_to_cancel);
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

        orders.emplace_back(orders.size(), side, type, price, volume);
    }

    // ----- Latency Benchmark Execution -----

    cout << "Running latency benchmark using " << num_orders << " orders..."
         << "\n";

    vector<long long> latencies;  // in nanoseconds
    latencies.reserve(num_orders);

    OrderBook latency_orderBook;
    for (const auto& order : orders) {
        auto start = chrono::high_resolution_clock::now();
        latency_orderBook.PlaceOrder(order);
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        latencies.push_back(static_cast<long long>(diff.count()));
    }

    // save latencies to a file for further analysis
    ofstream latency_file("latencies.txt");
    for (const auto& latency : latencies) {
        latency_file << latency << "\n";
    }
    latency_file.close();

    // ----- Results Output -----

    // Average latency
    double average_latency =
        accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
    cout << "- Average latency: " << average_latency << " ns" << "\n";

    // Min and Max latency
    auto [min_latency_it, max_latency_it] =
        ranges::minmax_element(latencies, std::less<>());
    cout << "- Min latency: " << *min_latency_it << " ns" << "\n";
    cout << "- Max latency: " << *max_latency_it << " ns" << "\n";

    // P50, P90, P99, P99.9 latencies
    ranges::sort(latencies, std::less<>());
    auto p50 = latencies[static_cast<size_t>(0.50 * (latencies.size() - 1))];
    auto p90 = latencies[static_cast<size_t>(0.90 * (latencies.size() - 1))];
    auto p99 = latencies[static_cast<size_t>(0.99 * (latencies.size() - 1))];
    auto p999 = latencies[static_cast<size_t>(0.999 * (latencies.size() - 1))];
    cout << "- P50 latency: " << p50 << " ns" << "\n";
    cout << "- P90 latency: " << p90 << " ns" << "\n";
    cout << "- P99 latency: " << p99 << " ns" << "\n";
    cout << "- P99.9 latency: " << p999 << " ns" << "\n";

    // ----- Throughput Benchmark Execution -----

    cout << "Running throughput benchmark using " << num_orders << " orders..."
         << "\n";

    OrderBook throughput_orderBook;
    auto throughput_start = chrono::high_resolution_clock::now();
    for (const auto& order : orders) {
        throughput_orderBook.PlaceOrder(order);
    }
    auto throughput_end = chrono::high_resolution_clock::now();
    auto total_duration = chrono::duration_cast<chrono::milliseconds>(
                              throughput_end - throughput_start)
                              .count();
    double throughput = static_cast<double>(num_orders) /
                        (static_cast<double>(total_duration) / 1000.0);
    cout << "- Throughput: " << throughput / 1e6 << "M orders/sec" << "\n";
}