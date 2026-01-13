#include "matching_engine/Order.hpp"
#include "common/Types.hpp"

// Constructor implementations

Order::Order() = default;

Order::Order(OrderID order_id, Timestamp timestamp, Side side,
             OrderType order_type, Price price, Volume volume)
    : order_id_(order_id),
      timestamp_(timestamp),
      side_(side),
      order_type_(order_type),
      price_(price),
      volume_(volume),
      filled_volume_(0) {}

// Getter method implementations

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

Volume Order::getFilledVolume() const {
    return filled_volume_;
}

Volume Order::getRemainingVolume() const {
    return volume_ - filled_volume_;
}

bool Order::isFilled() const {
    return filled_volume_ >= volume_;
}

// Setter method implementations

void Order::setVolume(Volume volume) {
    volume_ = volume;
}

void Order::addFilledVolume(Volume volume) {
    filled_volume_ += volume;
}