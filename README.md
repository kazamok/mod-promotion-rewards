# mod-promotion-rewards

## 🚀 개요

`mod-promotion-rewards` 모듈은 AzerothCore 기반 서버에서 플레이어의 홍보 활동에 대한 보상을 자동으로 지급하는 기능을 제공합니다. 이 모듈을 통해 서버 운영자는 특정 조건을 만족하는 플레이어에게 골드, 아이템 등을 우편으로 발송하여 홍보 활동을 장려할 수 있습니다.

## ✨ 주요 기능

-   **자동 보상 지급**: 설정된 주기에 따라 데이터베이스를 확인하고, 홍보 활동이 승인된 플레이어에게 자동으로 보상을 지급합니다.
-   **다양한 보상 유형**: 골드 및 특정 아이템을 보상으로 설정할 수 있습니다.
-   **커스터마이징 가능한 우편**: 보상 지급 시 발송되는 우편의 제목과 본문을 자유롭게 설정할 수 있습니다.
-   **유연한 설정**: 모듈 활성화 여부, 보상 확인 주기, 보상 내용 등을 세밀하게 제어할 수 있습니다.

## ⚙️ 설정

이 모듈의 모든 설정은 `conf/mod-promotion-rewards.conf.dist` 파일에서 관리됩니다. 서버의 필요에 따라 다음 설정들을 조정할 수 있습니다:

-   **`PromotionRewards.Enable`**: 모듈 활성화/비활성화 (0: 비활성화, 1: 활성화).
-   **`PromotionRewards.CheckInterval`**: 보상 확인 주기 (밀리초 단위).
-   **`PromotionRewards.RewardGold`**: 지급할 골드 양 (코퍼 단위).
-   **`PromotionRewards.RewardItemID`**: 지급할 아이템의 ID (0으로 설정 시 아이템 미지급).
-   **`PromotionRewards.RewardItemQuantity`**: 지급할 아이템의 수량.
-   **`PromotionRewards.MailSubject`**: 보상 우편의 제목.
-   **`PromotionRewards.MailBody`**: 보상 우편의 본문.

**예시:**
```ini
# 모듈 활성화 (0 = 비활성화, 1 = 활성화)
PromotionRewards.Enable = 1

# DB 확인 주기 (단위: 밀리초, 예: 3600000 = 1시간)
PromotionRewards.CheckInterval = 3600000

# 지급할 골드 (단위: 코퍼, 예: 100000 = 10골드)
PromotionRewards.RewardGold = 100000

# 지급할 아이템 ID (0 = 없음, 22칸가방)
PromotionRewards.RewardItemID = 38082

# 지급할 아이템 수량
PromotionRewards.RewardItemQuantity = 1

# 우편 발송 시 제목
PromotionRewards.MailSubject = "홍보 활동 보상이 도착했습니다!"

# 우편 발송 시 본문
PromotionRewards.MailBody = "참여해주셔서 감사합니다! 다음은 승인된 홍보 활동에 대한 보상입니다."
```

## 📦 설치

1.  `azerothcore/modules` 디렉토리 내에 `mod-promotion-rewards` 폴더를 복사합니다.
2.  CMake를 사용하여 AzerothCore를 재컴파일합니다. 모듈이 자동으로 감지되어 빌드에 포함됩니다.
3.  컴파일 후 생성된 `mod-promotion-rewards.conf.dist` 파일을 `etc/` 디렉토리로 복사하고 `.dist` 확장자를 제거하여 `mod-promotion-rewards.conf`로 이름을 변경합니다.
4.  `mod-promotion-rewards.conf` 파일을 서버 설정에 맞게 수정합니다.

## 🎮 사용법

모듈이 활성화되면, 설정된 `CheckInterval` 주기에 따라 데이터베이스를 확인하고, 홍보 활동이 승인된 플레이어에게 자동으로 보상이 지급됩니다. 플레이어는 게임 내 우편함을 통해 보상을 수령할 수 있습니다.

## 👥 크레딧
- Kazamok
- Gemini
- 모든 기여자들

## 📄 라이선스

이 프로젝트는 GPL-3.0 라이선스 하에 배포됩니다.
