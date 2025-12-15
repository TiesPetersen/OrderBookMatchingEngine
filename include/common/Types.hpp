#pragma once

#include <cstdint>

using OrderID = unsigned int;
using Price = double;
using Volume = unsigned int;
using Timestamp = uint64_t;

enum Side : uint8_t { BUY = 0, SELL = 1 };

enum OrderType : uint8_t { MARKET = 0, LIMIT = 1 };