#include <iostream>

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

OrderBook::OrderBook() = default;

void OrderBook::PlaceOrder(Order order) {
    // Add order to order book lookup
    orders_by_id_[order.getOrderId()] = order;

    // Add to buy or sell map queue
    if (order.getSide() == BUY) {
        buy_orders_by_price_[order.getPrice()].push_back(order);
    } else {
        sell_orders_by_price_[order.getPrice()].push_back(order);
    }

    std::cout << "Placing order with ID: " << order.getOrderId() << '\n';
}

Volume OrderBook::GetVolumeAtPrice(Price price, Side side) {
    // Initialize total volume
    Volume total_volume = 0;

    // Select the appropriate orders map based on side
    const auto& orders_map =
        (side == BUY) ? buy_orders_by_price_ : sell_orders_by_price_;

    // Find the price level in the selected map
    auto iter = orders_map.find(price);

    // If price level exists, sum the volumes of all orders at that level
    if (iter != orders_map.end()) {
        // Get the queue of orders at this price level
        const auto& orders_queue = iter->second;

        // Sum volumes of all orders at this price level
        for (const auto& order : orders_queue) {
            total_volume += order.getVolume();
        }
    }

    return total_volume;
}