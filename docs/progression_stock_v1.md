# Progression stock policy

New seller stock uses item-template metadata rather than an item-ID allowlist.
Any item in the existing tradable candidate pool may qualify. The original
binding, vendor, and auctionability filters still apply.

With progression stock enabled, the persisted player ceiling determines:

- Maximum required character level: ceiling plus the configured offset (5).
- Maximum required skill rank: ceiling times 5 plus 15, bounded by the
  configured maximum (450).
- For items with neither a level nor a skill requirement, maximum item level:
  ceiling plus 10, bounded by the configured maximum (200).
- Quality: common/uncommon/rare or lower; rare listings remain capped at 5%.

At ceiling 15, these rules allow required level 20, skill rank 90, and
requirement-free item level 25. Wild Steelbloom and additional low-tier recipes
can qualify without manual additions. Silk Cloth (item level 30) unlocks at
ceiling 20; Iron Ore (skill 125) at ceiling 22. Frostweave and Saronite remain
excluded at low progression. Item metadata is a proxy for tier; missing or
misclassified metadata can still admit unsuitable items.

Default house targets are Alliance 500–800, Horde 1000–1400, and Neutral
200–300 bot listings: 1700–2500 total. Seller batches remain capped at 50 per
house per cycle. Category weights favor trade goods (45), consumables (20),
equipment (20), recipes (10), and bags/quivers (5). These are selection weights,
not guaranteed percentages when candidates or duplicate limits restrict stock.
Equipment weights include uncommon and occasional rare pieces; recipe weights
include uncommon plans rather than relying only on common recipes.

Existing auctions are preserved. Buyer delays, seller budgets, duplicate caps,
and stack rules are unchanged. Both progression policy features remain opt-in.
Existing deployment overrides take precedence over these defaults. To adopt
this policy fully, raise inventory overrides and replace the previous fixed
item-level/skill caps (25/90) with the new maxima (200/450); the policy itself
continues to impose the lower progression-derived limits.
