#include <string>
#include "TestCases.hpp"
#include "TestRunner.hpp"
#include "TestUtils.hpp"

using namespace std;

int main() {
    TestRunner runner;

    // Run tests with fresh OrderBook each time
    runner.run("Empty Book", []() {
        OrderBook ob;
        TestEmptyBook(ob);
    });
    runner.run("Add Single Buy Limit", []() {
        OrderBook ob;
        TestAddSingleBuyLimit(ob);
    });
    runner.run("Add Single Sell Limit", []() {
        OrderBook ob;
        TestAddSingleSellLimit(ob);
    });
    runner.run("Match Simple Limit", []() {
        OrderBook ob;
        TestMatchSimple(ob);
    });
    runner.run("Partial Fill Incoming", []() {
        OrderBook ob;
        TestPartialFillIncoming(ob);
    });
    runner.run("Partial Fill Resting", []() {
        OrderBook ob;
        TestPartialFillResting(ob);
    });
    runner.run("Match Priority (FIFO)", []() {
        OrderBook ob;
        TestMatchPriority(ob);
    });
    runner.run("Match Best Price", []() {
        OrderBook ob;
        TestMatchBestPrice(ob);
    });
    runner.run("Market Order Buy", []() {
        OrderBook ob;
        TestMarketOrderBuy(ob);
    });
    runner.run("Market Order Sell", []() {
        OrderBook ob;
        TestMarketOrderSell(ob);
    });
    runner.run("Market Order No Liquidity", []() {
        OrderBook ob;
        TestMarketOrderNoLiquidity(ob);
    });
    runner.run("Cancel Order", []() {
        OrderBook ob;
        TestCancelOrder(ob);
    });
    runner.run("Cancel Non-Existent", []() {
        OrderBook ob;
        TestCancelNonExistent(ob);
    });
    runner.run("Multiple Price Levels", []() {
        OrderBook ob;
        TestMultiplePriceLevels(ob);
    });
    for (int v : {1, 5, 10, 50, 100}) {
        runner.run("Volume Test " + to_string(v), [v]() {
            OrderBook ob;
            TestVolumeParametric(ob, v);
        });
    }
    for (int p = 100; p <= 105; p++) {
        runner.run("Price Level Test " + to_string(p), [p]() {
            OrderBook ob;
            TestPriceLevelMatching(ob, p);
        });
    }
    runner.run("Cancel Middle Queue", []() {
        OrderBook ob;
        TestCancelMiddleQueue(ob);
    });
    runner.run("Self Match Different ID", []() {
        OrderBook ob;
        TestSelfMatchDifferentID(ob);
    });
    runner.run("Buy Higher Matches Sell Lower", []() {
        OrderBook ob;
        TestBuyHigherMatchesSellLower(ob);
    });
    runner.run("Sell Lower Matches Buy Higher", []() {
        OrderBook ob;
        TestSellLowerMatchesBuyHigher(ob);
    });
    runner.run("Accumulate Orders", []() {
        OrderBook ob;
        TestAccumulateOrders(ob);
    });
    runner.run("Zero Volume Order", []() {
        OrderBook ob;
        TestZeroVolumeOrder(ob);
    });
    runner.run("Market Order Partial Fill then Drop", []() {
        OrderBook ob;
        TestMarketOrderPartialFillThenDrop(ob);
    });
    runner.run("Market Order Clears Book", []() {
        OrderBook ob;
        TestMarketOrderClearsBook(ob);
    });
    runner.run("Interleaved Ops", []() {
        OrderBook ob;
        TestInterleavedOps(ob);
    });
    runner.run("Large Volume arithmetic", []() {
        OrderBook ob;
        TestLargeVolumeArithmetic(ob);
    });

    runner.summary();
    return runner.getFailed() > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
