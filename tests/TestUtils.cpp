#include "TestUtils.hpp"

// Global helper state
OrderID global_order_id = 1;

OrderID getNextId() {
    return global_order_id++;
}

Order createLimitOrder(Side side, Price price, Volume volume) {
    return Order(getNextId(), side, LIMIT, price, volume);
}

Order createMarketOrder(Side side, Volume volume) {
    return Order(getNextId(), side, MARKET, 0, volume);
}
