#ifndef AUCTION_HOUSE_POLICY_H
#define AUCTION_HOUSE_POLICY_H

#include <algorithm>
#include <array>
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

struct ProfessionTierEntry
{
    uint32_t itemId;
    uint32_t minimumSkill;
};

// Version 1: low-tier materials and recipes reviewed against the installed
// WotLK item_template. Unknown profession goods fail closed, including those
// with RequiredLevel and RequiredSkillRank both zero.
inline constexpr std::array<ProfessionTierEntry, 70> ProfessionTierV1 = {{
    { 723, 0 },  // Goretusk Liver
    { 729, 0 },  // Stringy Vulture Meat
    { 730, 0 },  // Murloc Eye
    { 731, 0 },  // Goretusk Snout
    { 765, 1 },  // Silverleaf
    { 769, 0 },  // Chunk of Boar Meat
    { 783, 1 },  // Light Hide
    { 785, 50 },  // Mageroyal
    { 1015, 0 },  // Lean Wolf Flank
    { 1080, 0 },  // Tough Condor Meat
    { 1081, 0 },  // Crisp Spider Meat
    { 1468, 0 },  // Murloc Fin
    { 2296, 0 },  // Great Goretusk Snout
    { 2318, 1 },  // Light Leather
    { 2447, 1 },  // Peacebloom
    { 2449, 15 },  // Earthroot
    { 2450, 70 },  // Briarthorn
    { 2452, 70 },  // Swiftthistle
    { 2589, 0 },  // Linen Cloth
    { 2592, 0 },  // Wool Cloth
    { 2672, 0 },  // Stringy Wolf Meat
    { 2673, 0 },  // Coyote Meat
    { 2674, 0 },  // Crawler Meat
    { 2675, 0 },  // Crawler Claw
    { 2677, 0 },  // Boar Ribs
    { 2770, 1 },  // Copper Ore
    { 2771, 65 },  // Tin Ore
    { 2775, 75 },  // Silver Ore
    { 2835, 1 },  // Rough Stone
    { 2836, 65 },  // Coarse Stone
    { 2840, 1 },  // Copper Bar
    { 2841, 65 },  // Bronze Bar
    { 2842, 75 },  // Silver Bar
    { 2881, 80 },  // Plans: Runed Copper Breastplate
    { 2886, 0 },  // Crag Boar Rib
    { 2924, 0 },  // Crocolisk Meat
    { 2934, 1 },  // Ruined Leather Scraps
    { 2996, 1 },  // Bolt of Linen Cloth
    { 2997, 75 },  // Bolt of Woolen Cloth
    { 3172, 0 },  // Boar Intestines
    { 3173, 0 },  // Bear Meat
    { 3174, 0 },  // Spider Ichor
    { 3470, 1 },  // Rough Grinding Stone
    { 3478, 65 },  // Coarse Grinding Stone
    { 3576, 65 },  // Tin Bar
    { 3609, 35 },  // Plans: Copper Chain Vest
    { 3820, 85 },  // Stranglekelp
    { 4231, 35 },  // Cured Light Hide
    { 5082, 1 },  // Thin Kodo Leather
    { 5465, 0 },  // Small Spider Leg
    { 5466, 0 },  // Scorpid Stinger
    { 5467, 0 },  // Kodo Meat
    { 5468, 0 },  // Soft Frenzy Flesh
    { 5469, 0 },  // Strider Meat
    { 5503, 0 },  // Clam Meat
    { 5784, 1 },  // Slimy Murloc Scale
    { 6289, 0 },  // Raw Longjaw Mud Snapper
    { 6291, 0 },  // Raw Brilliant Smallfish
    { 6303, 0 },  // Raw Slitherskin Mackerel
    { 6317, 0 },  // Raw Loch Frenzy
    { 6361, 0 },  // Raw Rainbow Fin Albacore
    { 6470, 1 },  // Deviate Scale
    { 6471, 1 },  // Perfect Deviate Scale
    { 6889, 0 },  // Small Egg
    { 10940, 1 },  // Strange Dust
    { 12223, 0 },  // Meaty Bat Wing
    { 22644, 0 },  // Crunchy Spider Leg
    { 22710, 1 },  // Bloodthistle
    { 27668, 0 },  // Lynx Meat
    { 27669, 0 }   // Bat Flesh
}};

inline bool ProfessionTierV1Allows(uint32_t itemId, uint32_t templateSkillRank, uint32_t skillCap)
{
    auto it = std::lower_bound(ProfessionTierV1.begin(), ProfessionTierV1.end(), itemId,
        [](ProfessionTierEntry const& entry, uint32_t id) { return entry.itemId < id; });
    return it != ProfessionTierV1.end() && it->itemId == itemId &&
        std::max(it->minimumSkill, templateSkillRank) <= skillCap;
}

inline bool StockEligible(uint32_t itemId, uint32_t quality, uint32_t requiredLevel,
    uint32_t requiredSkillRank, uint32_t itemLevel, bool professionItem, uint32_t ceiling,
    uint32_t requiredLevelOffset, uint32_t maxItemLevelWithoutRequirements, uint32_t professionSkillCap)
{
    if (!ceiling || requiredLevel > ceiling + requiredLevelOffset || quality > 3)
        return false;
    if (requiredSkillRank > professionSkillCap)
        return false;
    if (!requiredLevel && !requiredSkillRank && itemLevel > maxItemLevelWithoutRequirements)
        return false;
    return !professionItem || ProfessionTierV1Allows(itemId, requiredSkillRank, professionSkillCap);
}
}

#endif
