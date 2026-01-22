#pragma once

#include <cstdint>

using OrderID = uint64_t;  // max order ID 18,446,744,073,709,551,615
using Price = uint32_t;    // max price 4,294,967,295
using Volume = uint32_t;   // max volume 4,294,967,295

enum Side : uint8_t { BUY = 0, SELL = 1 };

enum OrderType : uint8_t { MARKET = 0, LIMIT = 1, CANCEL = 2 };

struct Trade {
    OrderID buy_order_id;
    OrderID sell_order_id;
    Price price;
    Volume volume;
};