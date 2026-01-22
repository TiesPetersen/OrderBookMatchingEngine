#pragma once

#include <cstdint>
#include "common/Types.hpp"

class Order {
   private:
    OrderID order_id_;
    Side side_;
    OrderType order_type_;
    Price price_;
    Volume volume_;
    Volume filled_volume_;
    OrderID cancel_order_id_;

   public:
    // Constructor
    Order();

    Order(OrderID order_id, Side side, OrderType order_type, Price price,
          Volume volume, OrderID cancel_order_id = INT64_MAX);

    // Getter methods
    OrderID getOrderId() const;
    Side getSide() const;
    OrderType getOrderType() const;
    Price getPrice() const;
    Volume getVolume() const;
    Volume getFilledVolume() const;
    Volume getRemainingVolume() const;
    OrderID getCancelOrderId() const;
    bool isFilled() const;

    // Setter methods
    void setVolume(Volume volume);
    void addFilledVolume(Volume volume);
};