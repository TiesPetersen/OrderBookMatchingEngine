#include <chrono>
#include <iostream>

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

int main() {
    // Create an instance of OrderBook
    OrderBook orderBook;

    // Example usage of OrderBook methods
    Order order1 =
        Order(1, std::chrono::steady_clock::now().time_since_epoch().count(),
              BUY, LIMIT, 100.0, 10);
    orderBook.PlaceOrder(order1);

    Volume volumeAtPrice = orderBook.GetVolumeAtPrice(100.0, BUY);
    std::cout << "Total volume at price 100.0 for BUY side: " << volumeAtPrice
              << '\n';
}