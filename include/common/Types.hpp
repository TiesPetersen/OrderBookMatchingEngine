#pragma once

#include <cstdint>

using OrderID = uint64_t;
using Price = uint64_t;
using Volume = uint64_t;
using Timestamp = uint64_t;

enum Side : uint8_t { BUY = 0, SELL = 1 };

enum OrderType : uint8_t { MARKET = 0, LIMIT = 1 };

struct Trade {
    OrderID buy_order_id;
    OrderID sell_order_id;
    Price price;
    Volume volume;
};