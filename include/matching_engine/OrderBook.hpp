#pragma once

#include <deque>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Order.hpp"
#include "common/Types.hpp"

using namespace std;

class OrderBook {
   private:
    map<Price, deque<shared_ptr<Order>>, greater<Price>> buy_orders_by_price_;
    map<Price, deque<shared_ptr<Order>>, less<Price>> sell_orders_by_price_;
    unordered_map<OrderID, shared_ptr<Order>> orders_by_id_;

    // Helpers
    vector<Trade> matchOrder(Order& order);
    Trade executeMatch(Order& incoming_order, Order& resting_order);
    bool canMatch(const Order& incoming, Price resting_price) const;
    void addOrderToBook(const Order& order);

   public:
    // Constructor
    OrderBook();

    // Core methods
    vector<Trade> PlaceOrder(Order order);
    void CancelOrder(OrderID orderId);

    // Query methods
    Volume GetVolumeAtPrice(Price price, Side side) const;
};