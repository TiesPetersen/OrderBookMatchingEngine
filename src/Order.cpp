#include "matching_engine/Order.hpp"
#include "common/Types.hpp"

Order::Order() = default;

Order::Order(OrderID order_id, Timestamp timestamp, Side side,
             OrderType order_type, Price price, Volume volume)
    : order_id_(order_id),
      timestamp_(timestamp),
      side_(side),
      order_type_(order_type),
      price_(price),
      volume_(volume) {}

OrderID Order::getOrderId() const {
    return order_id_;
}

Timestamp Order::getTimestamp() const {
    return timestamp_;
}

Side Order::getSide() const {
    return side_;
}

OrderType Order::getOrderType() const {
    return order_type_;
}

Price Order::getPrice() const {
    return price_;
}

Volume Order::getVolume() const {
    return volume_;
}

void Order::setVolume(Volume volume) {
    volume_ = volume;
}