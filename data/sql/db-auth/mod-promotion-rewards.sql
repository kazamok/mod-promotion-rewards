
/* 홍보 보상 받는 모듈에 포함될 테이블 추가 */

CREATE TABLE IF NOT EXISTS `acore_auth`.`promotions` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`user_id` INT NOT NULL,
	`character_name` VARCHAR(12) NOT NULL COLLATE 'utf8mb4_unicode_ci',   -- 보상받을 캐릭터 이름
	`promo_url` VARCHAR(255) NOT NULL COLLATE 'utf8mb4_unicode_ci',
	`created_at` TIMESTAMP NOT NULL DEFAULT (CURRENT_TIMESTAMP),
	`status` ENUM('pending','approved','rejected') NOT NULL DEFAULT 'pending' COLLATE 'utf8mb4_unicode_ci',
	`reward_sent` TINYINT(1) NOT NULL DEFAULT '0', -- 보상 지급 여부 (0: 미지급, 1: 지급 완료)
	`processed_at` TIMESTAMP NULL DEFAULT NULL, -- 보상 처리 시간
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `user_id` (`user_id`) USING BTREE
)
COLLATE='utf8mb4_unicode_ci'
ENGINE=InnoDB
;



