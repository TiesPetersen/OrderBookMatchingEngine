#include <algorithm>
#include <iostream>
#include <vector>

#include "common/Types.hpp"
#include "matching_engine/Order.hpp"
#include "matching_engine/OrderBook.hpp"

using namespace std;

OrderBook::OrderBook() = default;

vector<Trade> OrderBook::matchOrder(Order& order) {
    vector<Trade> trades;

    if (order.getSide() == BUY) {
        // Match against sell orders
        auto& opposite_book = sell_orders_by_price_;

        while (!order.isFilled() && !opposite_book.empty()) {
            // Get best price level from opposite side
            auto& [price, orders_at_price] = *opposite_book.begin();

            // Check if prices are matchable
            if (!canMatch(order, price)) {
                break;
            }

            // Get front order from queue
            shared_ptr<Order> resting_order = orders_at_price.front();

            // Execute trade
            Trade trade = executeMatch(order, *resting_order);
            trades.push_back(trade);

            // If resting order is filled, remove from book + hashmap
            if (resting_order->isFilled()) {
                orders_at_price.pop_front();
                orders_by_id_.erase(resting_order->getOrderId());

                if (orders_at_price.empty()) {
                    opposite_book.erase(opposite_book.begin());
                }
            }
        }
    } else {
        // Match against buy orders
        auto& opposite_book = buy_orders_by_price_;

        while (!order.isFilled() && !opposite_book.empty()) {
            // Get best price level from opposite side
            auto& [price, orders_at_price] = *opposite_book.begin();

            // Check if prices are matchable
            if (!canMatch(order, price)) {
                break;
            }

            // Get front order from queue
            shared_ptr<Order> resting_order = orders_at_price.front();

            // Execute trade
            Trade trade = executeMatch(order, *resting_order);
            trades.push_back(trade);

            // If resting order is filled, remove from book + hashmap
            if (resting_order->isFilled()) {
                orders_by_id_.erase(resting_order->getOrderId());
                orders_at_price.pop_front();

                if (orders_at_price.empty()) {
                    opposite_book.erase(opposite_book.begin());
                }
            }
        }
    }

    return trades;
}

Trade OrderBook::executeMatch(Order& incoming_order, Order& resting_order) {
    // Determine trade volume
    Volume trade_volume = min(incoming_order.getRemainingVolume(),
                              resting_order.getRemainingVolume());

    // Determine trade price (use resting order price)
    Price trade_price = resting_order.getPrice();

    // Update filled volumes
    incoming_order.addFilledVolume(trade_volume);
    resting_order.addFilledVolume(trade_volume);

    // Create and return Trade record
    Trade trade{.buy_order_id = (incoming_order.getSide() == BUY)
                                    ? incoming_order.getOrderId()
                                    : resting_order.getOrderId(),
                .sell_order_id = (incoming_order.getSide() == SELL)
                                     ? incoming_order.getOrderId()
                                     : resting_order.getOrderId(),
                .price = trade_price,
                .volume = trade_volume};

    return trade;
}

bool OrderBook::canMatch(const Order& incoming, Price resting_price) const {
    if (incoming.getOrderType() == MARKET) {
        return true;
    }

    if (incoming.getSide() == BUY) {
        return incoming.getPrice() >= resting_price;
    } else {
        return incoming.getPrice() <= resting_price;
    }
}

void OrderBook::addOrderToBook(const Order& order) {
    // Create a shared pointer for the order
    auto order_ptr = make_shared<Order>(order);

    // Add to appropriate book based on side
    if (order.getSide() == BUY) {
        buy_orders_by_price_[order.getPrice()].push_back(order_ptr);
    } else {
        sell_orders_by_price_[order.getPrice()].push_back(order_ptr);
    }

    // Add to hashmap
    orders_by_id_[order.getOrderId()] = order_ptr;
}

vector<Trade> OrderBook::PlaceOrder(Order order) {

    // Match the order against opposite side
    vector<Trade> trades = matchOrder(order);

    // If limit order has remaining volume, add to book + hashmap
    if (!order.isFilled() && order.getOrderType() == LIMIT) {
        addOrderToBook(order);
    }

    return trades;
}

void OrderBook::CancelOrder(OrderID orderId) {
    // Find the order in the hashmap
    auto it = orders_by_id_.find(orderId);
    if (it == orders_by_id_.end()) {
        // Order not found
        return;
    }
    shared_ptr<Order> order = it->second;

    Price price = order->getPrice();
    Side side = order->getSide();

    // Remove from order book
    if (side == BUY) {
        auto book_it = buy_orders_by_price_.find(price);
        if (book_it != buy_orders_by_price_.end()) {
            // Remove order from deque at this price level
            auto& orders_at_price = book_it->second;
            erase_if(orders_at_price,
                     [orderId](const shared_ptr<Order>& current_order) {
                         return current_order->getOrderId() == orderId;
                     });

            // If no more orders at this price, remove the price level
            if (orders_at_price.empty()) {
                buy_orders_by_price_.erase(book_it);
            }
        }
    } else {
        auto book_it = sell_orders_by_price_.find(price);
        if (book_it != sell_orders_by_price_.end()) {
            // Remove order from deque at this price level
            auto& orders_at_price = book_it->second;
            erase_if(orders_at_price,
                     [orderId](const shared_ptr<Order>& current_order) {
                         return current_order->getOrderId() == orderId;
                     });

            // If no more orders at this price, remove the price level
            if (orders_at_price.empty()) {
                sell_orders_by_price_.erase(book_it);
            }
        }
    }

    // Remove from hashmap
    orders_by_id_.erase(it);
}

bool OrderBook::ContainsOrder(OrderID orderId) const {
    return orders_by_id_.contains(orderId);
}

Volume OrderBook::GetVolumeAtPrice(Price price, Side side) const {
    // Initialize total volume
    Volume total_volume = 0;

    if (side == BUY) {
        auto it = buy_orders_by_price_.find(price);
        if (it != buy_orders_by_price_.end()) {
            const auto& orders_at_price = it->second;
            for (const auto& order : orders_at_price) {
                total_volume += order->getRemainingVolume();
            }
        }
    } else {
        auto it = sell_orders_by_price_.find(price);
        if (it != sell_orders_by_price_.end()) {
            const auto& orders_at_price = it->second;
            for (const auto& order : orders_at_price) {
                total_volume += order->getRemainingVolume();
            }
        }
    }

    return total_volume;
}

void OrderBook::GetOrderBookStats() const {
    cout << "Order Book Stats:\n";
    cout << "Buy Side:\n";
    for (const auto& [price, orders_at_price] : buy_orders_by_price_) {
        Volume total_volume = 0;
        for (const auto& order : orders_at_price) {
            total_volume += order->getRemainingVolume();
        }
        cout << "Price: " << price << ", Total Volume: " << total_volume
             << ", Orders: " << orders_at_price.size() << "\n";
    }

    cout << "Sell Side:\n";
    for (const auto& [price, orders_at_price] : sell_orders_by_price_) {
        Volume total_volume = 0;
        for (const auto& order : orders_at_price) {
            total_volume += order->getRemainingVolume();
        }
        cout << "Price: " << price << ", Total Volume: " << total_volume
             << ", Orders: " << orders_at_price.size() << "\n";
    }
}