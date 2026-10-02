# Progression profession stock mapping, version 1

`AuctionHousePolicy::ProfessionTierV1` is the allowlist for new profession
materials and recipes when progression stock rules are enabled. It currently
contains 70 item IDs spanning low-tier cloth, leather, herbs, ore and stone,
bars, enchanting dust, recipe plans, fishing catches, and cooking materials.
The exact IDs and conservative minimum skill floors are in the versioned array
in `src/AuctionHousePolicy.h`. The effective skill is the higher of the mapping
floor and `RequiredSkillRank`; stock is limited to skill 90 while the anchors'
ceiling is 13.

Unlisted profession goods are excluded, including items with zero required
level and zero required skill. The mapping assigns floors where templates do
not express a useful profession tier; natural drop and cooking materials may
have a zero floor. Later tiers require an explicit new mapping version and
review before they can enter new stock.

The staged `acore_world` copy was checked on 2026-10-02: all 70 IDs exist,
are unique, and their `entry`, `name`, `class`, `subclass`, `ItemLevel`, and
`RequiredSkillRank` fields were reviewed. No existing auction is changed by
this mapping.
