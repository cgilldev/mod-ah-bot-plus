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

    // Versioned tier mapping blocks unreviewed zero-requirement endgame goods.
    assert(ProfessionTierV1Allows(2589, 0, 90));
    assert(ProfessionTierV1Allows(2881, 80, 90));
    assert(!ProfessionTierV1Allows(2881, 80, 79));
    assert(!ProfessionTierV1Allows(999999, 0, 90));
    assert(!ProfessionTierV1Allows(2589, 91, 90));

    assert(StockEligible(2589, 1, 0, 0, 5, true, 13, 5, 25, 90));
    assert(!StockEligible(999999, 1, 0, 0, 5, true, 13, 5, 25, 90));
    assert(!StockEligible(2589, 1, 0, 0, 5, true, 0, 5, 25, 90));
    assert(!StockEligible(2589, 1, 19, 0, 5, true, 13, 5, 25, 90));
    assert(!StockEligible(2589, 4, 0, 0, 5, true, 13, 5, 25, 90));
    assert(!StockEligible(1234, 1, 0, 0, 26, false, 13, 5, 25, 90));
    assert(!StockEligible(15871, 2, 0, 200, 40, false, 15, 5, 25, 90));
    // Live rollout found these two newly listed items despite their template
    // skill ranks exceeding the configured cap; non-profession classes must
    // still obey the global skill-rank gate.
    assert(!StockEligible(4381, 2, 0, 140, 28, false, 15, 5, 25, 90));
    assert(!StockEligible(15870, 2, 0, 150, 30, false, 15, 5, 25, 90));
    assert(StockEligible(6811, 1, 0, 50, 20, false, 15, 5, 25, 90));
}
