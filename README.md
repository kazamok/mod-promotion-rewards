# 홍보 보상 모듈 (mod-promotion-rewards)

## 1. 모듈 소개

`mod-promotion-rewards`는 웹사이트와 연동하여, 관리자가 승인한 홍보 활동에 대해 플레이어에게 자동으로 보상을 지급하는 모듈입니다.

## 2. 주요 기능

*   데이터베이스의 `promotions` 테이블을 주기적으로 확인합니다.
*   `status`가 'approved'이고 `reward_sent`가 0인 항목을 찾아 보상을 지급합니다.
*   플레이어가 오프라인일 경우, 우편으로 보상을 발송합니다.
*   보상 지급 후 `reward_sent` 상태를 1로 변경하여 중복 지급을 방지합니다.
