CREATE TABLE IF NOT EXISTS `mod_ahbot_earned_bot_commitment` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `auction_id` int unsigned NOT NULL,
  `amount` bigint unsigned NOT NULL,
  `committed_at` int unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `idx_commitment_time` (`committed_at`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
