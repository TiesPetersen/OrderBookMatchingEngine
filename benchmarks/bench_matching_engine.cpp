#include <emmintrin.h>
#include <immintrin.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

const int kNumOrders =
    10'000'000;  // Total number of orders to generate and process

const int kMinPrice = 800'00;     // Min price in cents
const int kMaxPrice = 1200'00;    // Max price in cents
const int kStartPrice = 1000'00;  // Starting mid price in cents

const int kMaxCancelAttempts =
    20;  // Max attempts to find a valid target for CANCEL orders

int main() {
    // ----- Random Order Generation -----

    cout << "Generating " << kNumOrders << " random orders..." << "\n";

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
    orders.reserve(kNumOrders);

    // Shadow book to track active orders for generating valid CANCEL orders
    OrderBook shadow_book;
    vector<OrderID> active_limit_ids;
    active_limit_ids.reserve(kNumOrders);

    // Generate Orders
    while (orders.size() < kNumOrders) {
        // Simulate mid-price movement
        current_mid_price += price_difference(generator);
        current_mid_price = max<double>(current_mid_price, kMinPrice);
        current_mid_price = min<double>(current_mid_price, kMaxPrice);

        // Order type
        auto type = static_cast<OrderType>(type_distribution(generator));

        // CANCEL order
        if (type == CANCEL) {
            if (active_limit_ids.empty())
                continue;

            // Attempt to find a valid target
            size_t attempts = 0;

            while (attempts < kMaxCancelAttempts && !active_limit_ids.empty()) {
                // Pick random index
                uniform_int_distribution<size_t> dist(
                    0, active_limit_ids.size() - 1);
                size_t idx = dist(generator);
                OrderID target_id = active_limit_ids[idx];

                // Check if it really exists in the book (not filled)
                if (shadow_book.ContainsOrder(target_id)) {
                    // Found a valid limit order to cancel
                    orders.emplace_back(orders.size(), BUY, CANCEL, 0, 0,
                                        target_id);
                    shadow_book.CancelOrder(target_id);

                    // Remove from active list
                    active_limit_ids[idx] = active_limit_ids.back();
                    active_limit_ids.pop_back();
                    break;
                }

                // It was already filled, remove from active list and try again
                active_limit_ids[idx] = active_limit_ids.back();
                active_limit_ids.pop_back();

                attempts++;
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

        // Update Shadow State
        shadow_book.PlaceOrder(orders.back());

        if (type == LIMIT) {
            active_limit_ids.push_back(orders.back().getOrderId());
        }
    }

    // ----- Latency Benchmark Execution -----

    // Warm-up
    cout << "Populating order book by simulating " << kNumOrders / 2
         << " orders..."
         << "\n";

    OrderBook latency_orderBook;
    for (int i = 0; i < kNumOrders / 2; i++) {
        if (orders[i].getOrderType() != CANCEL) {
            latency_orderBook.PlaceOrder(orders[i]);
        } else {
            latency_orderBook.CancelOrder(orders[i].getCancelOrderId());
        }
    }

    // Latency measurement
    cout << "Running latency benchmark using the remaining " << kNumOrders / 2
         << " orders..."
         << "\n";

    vector<long long> latencies;  // in nanoseconds
    latencies.reserve(kNumOrders / 2);

    long long total_checksum = 0;  // To prevent compiler optimizations

    for (int i = kNumOrders / 2; i < kNumOrders; i++) {
        // Force cold cache for the order data
        _mm_clflush(&orders[i]);
        _mm_mfence();

        auto start = chrono::high_resolution_clock::now();
        vector<Trade> trades;
        if (orders[i].getOrderType() != CANCEL) {
            trades = latency_orderBook.PlaceOrder(orders[i]);
        } else {
            latency_orderBook.CancelOrder(orders[i].getCancelOrderId());
        }
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        latencies.push_back(static_cast<long long>(diff.count()));

        // Prevent compiler optimization by using the trades result in some way
        total_checksum += trades.size();
    }

    cout << "Total checksum (to prevent optimization, ignore this number): "
         << total_checksum << "\n";

    // Save latencies to a file for further analysis
    ofstream latency_file("latencies.txt");
    for (const auto& latency : latencies) {
        latency_file << latency << "\n";
    }
    latency_file.close();

    // ----- Latency results -----

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

    // Warm-up
    cout << "Populating order book by simulating " << kNumOrders / 2
         << " orders..." << "\n";

    OrderBook throughput_orderBook;
    for (int i = 0; i < kNumOrders / 2; i++) {
        if (orders[i].getOrderType() != CANCEL) {
            throughput_orderBook.PlaceOrder(orders[i]);
        } else {
            throughput_orderBook.CancelOrder(orders[i].getCancelOrderId());
        }
    }

    // Throughput measurement
    cout << "Running throughput benchmark using the remaining "
         << kNumOrders / 2 << " orders..."
         << "\n";

    auto throughput_start = chrono::high_resolution_clock::now();
    for (int i = kNumOrders / 2; i < kNumOrders; i++) {
        // Force cold cache for the order data
        _mm_clflush(&orders[i]);
        _mm_mfence();

        if (orders[i].getOrderType() != CANCEL) {
            throughput_orderBook.PlaceOrder(orders[i]);
        } else {
            throughput_orderBook.CancelOrder(orders[i].getCancelOrderId());
        }
    }
    auto throughput_end = chrono::high_resolution_clock::now();

    // ----- Throughput results -----

    auto total_duration = chrono::duration_cast<chrono::milliseconds>(
                              throughput_end - throughput_start)
                              .count();
    double throughput = static_cast<double>(kNumOrders / 2) /
                        (static_cast<double>(total_duration) / 1000.0);
    cout << "- Throughput: " << throughput / 1e6 << "M orders/sec" << "\n";
}