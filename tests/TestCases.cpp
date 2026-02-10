#include "TestCases.hpp"
#include "TestUtils.hpp"

using namespace std;

void TestEmptyBook(OrderBook& ob) {
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 0);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 0);
    ASSERT_FALSE(ob.ContainsOrder(1));
}

void TestAddSingleBuyLimit(OrderBook& ob) {
    Order order = createLimitOrder(BUY, 100, 10);
    auto trades = ob.PlaceOrder(order);

    ASSERT_EQ(trades.size(), 0);
    ASSERT_TRUE(ob.ContainsOrder(order.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 10);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 0);
}

void TestAddSingleSellLimit(OrderBook& ob) {
    Order order = createLimitOrder(SELL, 150, 20);
    auto trades = ob.PlaceOrder(order);

    ASSERT_EQ(trades.size(), 0);
    ASSERT_TRUE(ob.ContainsOrder(order.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(150, SELL), 20);
    ASSERT_EQ(ob.GetVolumeAtPrice(150, BUY), 0);
}

void TestMatchSimple(OrderBook& ob) {
    // Sell 10 @ 100
    Order sell = createLimitOrder(SELL, 100, 10);
    ob.PlaceOrder(sell);

    // Buy 10 @ 100
    Order buy = createLimitOrder(BUY, 100, 10);
    auto trades = ob.PlaceOrder(buy);

    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].volume, 10);
    ASSERT_EQ(trades[0].price, 100);

    // Both should be filled and removed
    ASSERT_FALSE(ob.ContainsOrder(sell.getOrderId()));
    ASSERT_FALSE(ob.ContainsOrder(buy.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 0);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 0);
}

void TestPartialFillIncoming(OrderBook& ob) {
    // Sell 10 @ 100
    Order sell = createLimitOrder(SELL, 100, 10);
    ob.PlaceOrder(sell);

    // Buy 15 @ 100 (matches 10, 5 remaining)
    Order buy = createLimitOrder(BUY, 100, 15);
    auto trades = ob.PlaceOrder(buy);

    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].volume, 10);

    // Sell order filled and removed
    ASSERT_FALSE(ob.ContainsOrder(sell.getOrderId()));

    // Buy order remains with 5 volume
    ASSERT_TRUE(ob.ContainsOrder(buy.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 5);
}

void TestPartialFillResting(OrderBook& ob) {
    // Sell 20 @ 100
    Order sell = createLimitOrder(SELL, 100, 20);
    ob.PlaceOrder(sell);

    // Buy 10 @ 100
    Order buy = createLimitOrder(BUY, 100, 10);
    auto trades = ob.PlaceOrder(buy);

    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].volume, 10);

    // Buy order matched fully
    ASSERT_FALSE(ob.ContainsOrder(buy.getOrderId()));

    // Sell order remains with 10 volume
    ASSERT_TRUE(ob.ContainsOrder(sell.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 10);
}

void TestMatchPriority(OrderBook& ob) {
    // Sell 10 @ 100 (ID 1)
    Order sell1 = createLimitOrder(SELL, 100, 10);
    ob.PlaceOrder(sell1);

    // Sell 10 @ 100 (ID 2)
    Order sell2 = createLimitOrder(SELL, 100, 10);
    ob.PlaceOrder(sell2);

    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 20);

    // Buy 15 @ 100
    Order buy = createLimitOrder(BUY, 100, 15);
    auto trades = ob.PlaceOrder(buy);

    ASSERT_EQ(trades.size(), 2);
    // Should match sell1 first (FIFO)
    ASSERT_EQ(trades[0].sell_order_id, sell1.getOrderId());
    ASSERT_EQ(trades[0].volume, 10);

    // Then sell2
    ASSERT_EQ(trades[1].sell_order_id, sell2.getOrderId());
    ASSERT_EQ(trades[1].volume, 5);  // 15 - 10 = 5

    ASSERT_FALSE(ob.ContainsOrder(sell1.getOrderId()));
    ASSERT_TRUE(ob.ContainsOrder(sell2.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 5);
}

void TestMatchBestPrice(OrderBook& ob) {
    // Sell 10 @ 100
    Order sell1 = createLimitOrder(SELL, 100, 10);
    ob.PlaceOrder(sell1);

    // Sell 10 @ 99 (Better price for buyer)
    Order sell2 = createLimitOrder(SELL, 99, 10);
    ob.PlaceOrder(sell2);

    // Buy 10 @ 100
    Order buy = createLimitOrder(BUY, 100, 10);
    auto trades = ob.PlaceOrder(buy);

    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].sell_order_id, sell2.getOrderId());
    ASSERT_EQ(trades[0].price, 99);
}

void TestMarketOrderBuy(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(SELL, 100, 10));
    ob.PlaceOrder(createLimitOrder(SELL, 101, 10));

    Order mkt = createMarketOrder(BUY, 15);
    auto trades = ob.PlaceOrder(mkt);

    ASSERT_EQ(trades.size(), 2);
    ASSERT_EQ(trades[0].price, 100);
    ASSERT_EQ(trades[0].volume, 10);
    ASSERT_EQ(trades[1].price, 101);
    ASSERT_EQ(trades[1].volume, 5);

    ASSERT_EQ(ob.GetVolumeAtPrice(101, SELL), 5);
}

void TestMarketOrderSell(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(BUY, 100, 10));
    ob.PlaceOrder(createLimitOrder(BUY, 99, 10));

    Order mkt = createMarketOrder(SELL, 15);
    auto trades = ob.PlaceOrder(mkt);

    ASSERT_EQ(trades.size(), 2);
    ASSERT_EQ(trades[0].price, 100);
    ASSERT_EQ(trades[0].volume, 10);
    ASSERT_EQ(trades[1].price, 99);
    ASSERT_EQ(trades[1].volume, 5);
}

void TestMarketOrderNoLiquidity(OrderBook& ob) {
    Order mkt = createMarketOrder(BUY, 10);
    auto trades = ob.PlaceOrder(mkt);

    ASSERT_EQ(trades.size(), 0);
    ASSERT_FALSE(ob.ContainsOrder(mkt.getOrderId()));
}

void TestCancelOrder(OrderBook& ob) {
    Order order = createLimitOrder(BUY, 100, 10);
    ob.PlaceOrder(order);

    ASSERT_TRUE(ob.ContainsOrder(order.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 10);

    ob.CancelOrder(order.getOrderId());

    ASSERT_FALSE(ob.ContainsOrder(order.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 0);
}

void TestCancelNonExistent(OrderBook& ob) {
    ob.CancelOrder(99999);
    Order order = createLimitOrder(BUY, 100, 10);
    ob.PlaceOrder(order);
    ob.CancelOrder(99999);
    ASSERT_TRUE(ob.ContainsOrder(order.getOrderId()));
}

void TestMultiplePriceLevels(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(SELL, 100, 10));
    ob.PlaceOrder(createLimitOrder(SELL, 101, 10));
    ob.PlaceOrder(createLimitOrder(SELL, 102, 10));

    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 10);
    ASSERT_EQ(ob.GetVolumeAtPrice(101, SELL), 10);
    ASSERT_EQ(ob.GetVolumeAtPrice(102, SELL), 10);

    // Buy sweeps levels
    Order sweep = createLimitOrder(BUY, 102, 25);
    auto trades = ob.PlaceOrder(sweep);

    ASSERT_EQ(trades.size(), 3);
    ASSERT_EQ(trades[0].price, 100);
    ASSERT_EQ(trades[1].price, 101);
    ASSERT_EQ(trades[2].price, 102);
    ASSERT_EQ(trades[2].volume, 5);

    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 0);
    ASSERT_EQ(ob.GetVolumeAtPrice(101, SELL), 0);
    ASSERT_EQ(ob.GetVolumeAtPrice(102, SELL), 5);
}

void TestVolumeParametric(OrderBook& ob, int v) {
    ob.PlaceOrder(createLimitOrder(BUY, 100, v));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), v);
}

void TestPriceLevelMatching(OrderBook& ob, int p) {
    ob.PlaceOrder(createLimitOrder(SELL, p, 10));
    Order buy = createLimitOrder(BUY, p, 10);
    auto trades = ob.PlaceOrder(buy);
    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].price, p);
}

void TestCancelMiddleQueue(OrderBook& ob) {
    Order o1 = createLimitOrder(BUY, 100, 10);
    Order o2 = createLimitOrder(BUY, 100, 10);
    Order o3 = createLimitOrder(BUY, 100, 10);
    ob.PlaceOrder(o1);
    ob.PlaceOrder(o2);
    ob.PlaceOrder(o3);

    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 30);

    ob.CancelOrder(o2.getOrderId());

    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 20);
    ASSERT_TRUE(ob.ContainsOrder(o1.getOrderId()));
    ASSERT_FALSE(ob.ContainsOrder(o2.getOrderId()));
    ASSERT_TRUE(ob.ContainsOrder(o3.getOrderId()));

    // Matching should hit o1 then o3
    Order sell = createLimitOrder(SELL, 100, 20);
    auto trades = ob.PlaceOrder(sell);
    ASSERT_EQ(trades.size(), 2);
    ASSERT_EQ(trades[0].buy_order_id, o1.getOrderId());
    ASSERT_EQ(trades[1].buy_order_id, o3.getOrderId());
}

void TestSelfMatchDifferentID(OrderBook& ob) {
    Order s = createLimitOrder(SELL, 100, 10);
    Order b = createLimitOrder(BUY, 100, 10);
    ob.PlaceOrder(s);
    auto trades = ob.PlaceOrder(b);
    ASSERT_EQ(trades.size(), 1);
}

void TestBuyHigherMatchesSellLower(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(SELL, 90, 10));
    Order buy = createLimitOrder(BUY, 100, 10);
    auto trades = ob.PlaceOrder(buy);
    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].price, 90);
}

void TestSellLowerMatchesBuyHigher(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(BUY, 110, 10));
    Order sell = createLimitOrder(SELL, 100, 10);
    auto trades = ob.PlaceOrder(sell);
    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].price, 110);
}

void TestAccumulateOrders(OrderBook& ob) {
    for (int i = 0; i < 50; ++i) {
        ob.PlaceOrder(createLimitOrder(BUY, 100, 1));
    }
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 50);

    auto trades = ob.PlaceOrder(createLimitOrder(SELL, 100, 50));
    ASSERT_EQ(trades.size(), 50);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 0);
}

void TestZeroVolumeOrder(OrderBook& ob) {
    Order z = createLimitOrder(BUY, 100, 0);
    ob.PlaceOrder(z);
    ASSERT_FALSE(ob.ContainsOrder(z.getOrderId()));
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 0);
}

void TestMarketOrderPartialFillThenDrop(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(SELL, 100, 5));
    Order mkt = createMarketOrder(BUY, 10);
    auto trades = ob.PlaceOrder(mkt);

    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].volume, 5);
    ASSERT_FALSE(ob.ContainsOrder(mkt.getOrderId()));
}

void TestMarketOrderClearsBook(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(SELL, 100, 10));
    ob.PlaceOrder(createLimitOrder(SELL, 101, 10));

    Order mkt = createMarketOrder(BUY, 100);
    auto trades = ob.PlaceOrder(mkt);

    ASSERT_EQ(trades.size(), 2);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, SELL), 0);
    ASSERT_EQ(ob.GetVolumeAtPrice(101, SELL), 0);
    ASSERT_EQ(trades[0].volume + trades[1].volume, 20);
}

void TestInterleavedOps(OrderBook& ob) {
    ob.PlaceOrder(createLimitOrder(BUY, 100, 10));
    ob.PlaceOrder(createLimitOrder(SELL, 101, 10));

    // This should not match
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 10);
    ASSERT_EQ(ob.GetVolumeAtPrice(101, SELL), 10);

    // Add bridge
    Order bridge = createLimitOrder(SELL, 100, 5);
    ob.PlaceOrder(bridge);

    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), 5);
    ASSERT_FALSE(ob.ContainsOrder(bridge.getOrderId()));
}

void TestLargeVolumeArithmetic(OrderBook& ob) {
    Volume large = 1000000;
    ob.PlaceOrder(createLimitOrder(BUY, 100, large));
    ob.PlaceOrder(createLimitOrder(BUY, 100, large));

    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), large * 2);

    auto trades = ob.PlaceOrder(createLimitOrder(SELL, 100, large));
    ASSERT_EQ(trades.size(), 1);
    ASSERT_EQ(trades[0].volume, large);
    ASSERT_EQ(ob.GetVolumeAtPrice(100, BUY), large);
}
