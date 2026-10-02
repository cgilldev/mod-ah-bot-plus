#include "../src/AuctionHousePolicy.h"

#include <cassert>
#include <cstdint>
#include <limits>

using namespace AuctionHousePolicy;

int main()
{
    // A fair starting bid remains a bid when the buyout carries a premium.
    PurchaseDecision bid = ChoosePurchase(100, 0, 200, 200, false);
    assert(bid.kind == PurchaseKind::Bid && bid.amount == 100);
    PurchaseDecision buyout = ChoosePurchase(100, 0, 110, 200, false);
    assert(buyout.kind == PurchaseKind::Buyout && buyout.amount == 110);
    assert(ChoosePurchase(300, 0, 0, 200, false).kind == PurchaseKind::Skip);

    // Current bids, whether human or an existing bot bid, are left to settle.
    assert(ChoosePurchase(100, 50, 200, 200, false).kind == PurchaseKind::Skip);
    assert(ReviewReady(1800, 1799) == false);
    assert(ReviewReady(1800, 1800) == true);

    // A full stack is compared with a full vendor stack, and the no-vendor
    // sentinel must not be treated as a huge vendor value.
    assert(VendorAllows(500, 100, 5));
    assert(!VendorAllows(501, 100, 5));
    assert(VendorAllows(999999, std::numeric_limits<uint32_t>::max(), 1));

    assert(WithinSellerBudget(5, 10000, 3000, 6, 13000));
    assert(!WithinSellerBudget(6, 0, 1, 6, 13000));
    assert(!WithinSellerBudget(5, 10000, 3001, 6, 13000));

    assert(CanListDuplicate(2));
    assert(!CanListDuplicate(3));
    assert(!CanListRare(0, 0));
    assert(CanListRare(0, 19));
    assert(!CanListRare(1, 20));

    assert(SmallStack(20, true, false, 0) == 1);
    assert(SmallStack(20, true, false, 1) == 5);
    assert(SmallStack(20, true, false, 2) == 10);
    assert(SmallStack(6, false, true, 2) == 5);
    assert(SmallStack(3, true, false, 2) == 1);

    // Low-level materials and recipes are admitted by metadata, even when they
    // were absent from the former allowlist (Wild Steelbloom, for example).
    assert(StockEligible(1, 0, 0, 5, 15, 5, 200, 450));   // Linen Cloth
    assert(StockEligible(1, 0, 75, 22, 15, 5, 200, 450)); // Wild Steelbloom
    assert(StockEligible(2, 0, 80, 18, 15, 5, 200, 450)); // Copper recipe
    assert(!StockEligible(1, 0, 0, 5, 0, 5, 200, 450)); // Missing progression
    assert(!StockEligible(1, 21, 0, 20, 15, 5, 200, 450));
    assert(!StockEligible(4, 15, 0, 20, 15, 5, 200, 450));

    // Higher material tiers unlock as the players progress, without new IDs.
    assert(!StockEligible(1, 0, 0, 30, 15, 5, 200, 450)); // Silk Cloth
    assert(StockEligible(1, 0, 0, 30, 20, 5, 200, 450));
    assert(!StockEligible(1, 0, 125, 30, 20, 5, 200, 450)); // Iron Ore
    assert(StockEligible(1, 0, 125, 30, 22, 5, 200, 450));
    assert(!StockEligible(1, 0, 0, 70, 15, 5, 200, 450)); // Frostweave
    assert(StockEligible(1, 0, 0, 70, 60, 5, 200, 450));
    assert(!StockEligible(1, 0, 400, 75, 15, 5, 200, 450)); // Saronite
    assert(StockEligible(1, 0, 400, 75, 80, 5, 200, 450));

    // Explicit configuration can still impose stricter limits.
    assert(!StockEligible(1, 0, 0, 30, 40, 5, 25, 450));
    assert(!StockEligible(1, 0, 125, 30, 40, 5, 200, 90));
    // Every class observes the skill cap, including engineering equipment.
    assert(!StockEligible(2, 0, 140, 28, 15, 5, 200, 450));
    assert(!StockEligible(2, 0, 150, 30, 15, 5, 200, 450));
    assert(StockEligible(1, 0, 50, 20, 15, 5, 200, 450));
}
