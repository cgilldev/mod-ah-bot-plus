#ifndef AUCTION_HOUSE_POLICY_H
#define AUCTION_HOUSE_POLICY_H

#include <algorithm>
#include <cstdint>
#include <limits>

namespace AuctionHousePolicy
{
enum class PurchaseKind : uint8_t
{
    Skip,
    Bid,
    Buyout
};

struct PurchaseDecision
{
    PurchaseKind kind;
    uint64_t amount;
};

inline PurchaseDecision ChoosePurchase(uint64_t startingBid, uint64_t currentBid,
    uint64_t buyout, uint64_t willingToPay, bool bidMaximum)
{
    // A bid already placed by anyone remains valid; the buyer never displaces it.
    if (currentBid || !willingToPay)
        return { PurchaseKind::Skip, 0 };

    bool fairBid = startingBid && startingBid <= willingToPay;
    bool fairBuyout = buyout && buyout <= willingToPay;
    if (fairBid && (!fairBuyout || startingBid * 5 <= buyout * 4))
    {
        uint64_t amount = bidMaximum ? willingToPay : startingBid;
        if (buyout)
            amount = std::min(amount, buyout - 1);
        return amount >= startingBid ? PurchaseDecision{ PurchaseKind::Bid, amount }
                                    : PurchaseDecision{ PurchaseKind::Skip, 0 };
    }
    if (fairBuyout)
        return { PurchaseKind::Buyout, buyout };

    return { PurchaseKind::Skip, 0 };
}

inline bool VendorAllows(uint64_t commitment, uint32_t vendorUnitPrice, uint32_t stackCount)
{
    return !vendorUnitPrice || vendorUnitPrice == std::numeric_limits<uint32_t>::max() ||
        commitment <= static_cast<uint64_t>(vendorUnitPrice) * stackCount;
}

inline bool WithinSellerBudget(uint32_t count, uint64_t spent, uint64_t proposed,
    uint32_t maxCount, uint64_t maxCopper)
{
    return count < maxCount && proposed <= maxCopper && spent <= maxCopper - proposed;
}

inline bool ReviewReady(uint32_t reviewAfter, uint32_t now)
{
    return now >= reviewAfter;
}

inline bool CanListDuplicate(uint32_t botListingsForTemplate)
{
    return botListingsForTemplate < 3;
}

inline bool CanListRare(uint32_t rareListings, uint32_t allBotListings)
{
    return static_cast<uint64_t>(rareListings + 1) * 100 <=
        static_cast<uint64_t>(allBotListings + 1) * 5;
}

inline uint32_t SmallStack(uint32_t legalMaximum, bool material, bool consumable, uint32_t choice)
{
    if (!legalMaximum)
        return 1;
    if (!material && !consumable)
        return 1;

    uint32_t desired = choice == 0 ? 1 : (choice == 1 ? 5 : (material ? 10 : 5));
    return desired <= legalMaximum ? desired : 1;
}

// Use the installed item metadata instead of an item-ID allowlist. Limits rise
// with the persisted player ceiling; configured maxima can keep them lower.
inline bool StockEligible(uint32_t quality, uint32_t requiredLevel, uint32_t requiredSkillRank,
    uint32_t itemLevel, uint32_t ceiling, uint32_t requiredLevelOffset,
    uint32_t maxItemLevelWithoutRequirements, uint32_t professionSkillCap)
{
    if (!ceiling || requiredLevel > static_cast<uint64_t>(ceiling) + requiredLevelOffset || quality > 3)
        return false;

    uint64_t skillLimit = std::min<uint64_t>(professionSkillCap, static_cast<uint64_t>(ceiling) * 5 + 15);
    if (requiredSkillRank > skillLimit)
        return false;

    uint64_t itemLevelLimit = std::min<uint64_t>(maxItemLevelWithoutRequirements,
        static_cast<uint64_t>(ceiling) + 10);
    if (!requiredLevel && !requiredSkillRank && itemLevel > itemLevelLimit)
        return false;

    return true;
}
}

#endif
