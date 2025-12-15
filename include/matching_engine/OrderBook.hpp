#pragma once

#include <deque>
#include <map>
#include <unordered_map>
#include "Order.hpp"
#include "common/Types.hpp"

class OrderBook {
   private:
    std::map<Price, std::deque<Order>> buy_orders_by_price_;
    std::map<Price, std::deque<Order>> sell_orders_by_price_;
    std::unordered_map<OrderID, Order> orders_by_id_;

   public:
    OrderBook();

    void PlaceOrder(Order order);

    void CancelOrder(OrderID orderId);

    Volume GetVolumeAtPrice(Price price, Side side);
};