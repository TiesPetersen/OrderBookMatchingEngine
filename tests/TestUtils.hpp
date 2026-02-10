#pragma once

#include <stdexcept>
#include <string>
#include "common/Types.hpp"
#include "matching_engine/Order.hpp"

using namespace std;

// Assert macros
#define ASSERT_TRUE(condition) \
    if (!(condition))          \
        throw runtime_error("Assertion failed: " #condition);

#define ASSERT_FALSE(condition) \
    if ((condition))            \
        throw runtime_error("Assertion failed: " #condition " is true");

#define ASSERT_EQ(a, b)                                                      \
    if ((a) != (b))                                                          \
        throw runtime_error("Assertion failed: " #a " == " #b " (Values: " + \
                            to_string(a) + " vs " + to_string(b) + ")");

// Helper Declarations
OrderID getNextId();
Order createLimitOrder(Side side, Price price, Volume volume);
Order createMarketOrder(Side side, Volume volume);
