#pragma once

#include "common/Types.hpp"

class Order {
   private:
    OrderID order_id_;
    Timestamp timestamp_;
    Side side_;
    OrderType order_type_;
    Price price_;
    Volume volume_;

   public:
    // Constructor
    Order();

    Order(OrderID order_id, Timestamp timestamp, Side side,
          OrderType order_type, Price price, Volume volume);

    // Getter methods
    OrderID getOrderId() const;
    Timestamp getTimestamp() const;
    Side getSide() const;
    OrderType getOrderType() const;
    Price getPrice() const;
    Volume getVolume() const;

    // Setter methods
    void setVolume(Volume volume);
};