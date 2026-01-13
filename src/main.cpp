#include <chrono>
#include <iostream>

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

int main() {
    // Create an instance of OrderBook
    OrderBook orderBook;

    Order order1 = Order(1, BUY, LIMIT, 100.0, 10);
    orderBook.PlaceOrder(order1);

    Order order2 = Order(2, SELL, LIMIT, 100.0, 5);

    Volume volumeAtPrice = orderBook.GetVolumeAtPrice(100.0, BUY);
    std::cout << "Total volume at price 100.0 for BUY side: " << volumeAtPrice
              << '\n';
}