CREATE TABLE IF NOT EXISTS `mod_ahbot_player_auction_review` (
  `auction_id` int unsigned NOT NULL,
  `first_seen` int unsigned NOT NULL,
  `review_after` int unsigned NOT NULL,
  PRIMARY KEY (`auction_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `mod_ahbot_buyer_commitment` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `seller_guid` int unsigned NOT NULL,
  `auction_id` int unsigned NOT NULL,
  `amount` bigint unsigned NOT NULL,
  `committed_at` int unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_seller_time` (`seller_guid`, `committed_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
