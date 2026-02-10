#pragma once
#include "matching_engine/OrderBook.hpp"

void TestEmptyBook(OrderBook& ob);
void TestAddSingleBuyLimit(OrderBook& ob);
void TestAddSingleSellLimit(OrderBook& ob);
void TestMatchSimple(OrderBook& ob);
void TestPartialFillIncoming(OrderBook& ob);
void TestPartialFillResting(OrderBook& ob);
void TestMatchPriority(OrderBook& ob);
void TestMatchBestPrice(OrderBook& ob);
void TestMarketOrderBuy(OrderBook& ob);
void TestMarketOrderSell(OrderBook& ob);
void TestMarketOrderNoLiquidity(OrderBook& ob);
void TestCancelOrder(OrderBook& ob);
void TestCancelNonExistent(OrderBook& ob);
void TestMultiplePriceLevels(OrderBook& ob);

void TestVolumeParametric(OrderBook& ob, int v);
void TestPriceLevelMatching(OrderBook& ob, int p);
void TestCancelMiddleQueue(OrderBook& ob);
void TestSelfMatchDifferentID(OrderBook& ob);
void TestBuyHigherMatchesSellLower(OrderBook& ob);
void TestSellLowerMatchesBuyHigher(OrderBook& ob);
void TestAccumulateOrders(OrderBook& ob);
void TestZeroVolumeOrder(OrderBook& ob);
void TestMarketOrderPartialFillThenDrop(OrderBook& ob);
void TestMarketOrderClearsBook(OrderBook& ob);
void TestInterleavedOps(OrderBook& ob);
void TestLargeVolumeArithmetic(OrderBook& ob);
