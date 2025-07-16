#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "Config.h"
#include "World.h"
#include "WorldSessionMgr.h"
#include "DatabaseEnv.h"
#include "Mail.h"
#include "CharacterCache.h" // CharacterCache 헤더 추가
#include "Item.h" // Item.h 헤더 추가
#include "ObjectMgr.h" // ObjectMgr.h 헤더 추가
#include <algorithm> // std::transform 사용을 위해 추가
#include <cctype>    // ::tolower 사용을 위해 추가

// 전역 변수 선언
bool g_promotionRewardsEnabled = false;
uint32 g_promotionRewardsCheckInterval = 300;
uint32 g_promotionRewardsGold = 0;
uint32 g_promotionRewardsItemId = 0;
uint32 g_promotionRewardsItemQuantity = 1;
std::string g_promotionRewardsMailSubject = "";
std::string g_promotionRewardsMailBody = "";

// 다음 DB 확인까지 남은 시간
time_t g_timeUntilNextCheck = 0;

// 설정 로드 함수
void LoadPromotionRewardsConfig()
{
    // 기본값 설정 여기 수정해도 소용없음, 컨픽을 수정해야함
    g_promotionRewardsEnabled = sConfigMgr->GetOption<bool>("PromotionRewards.Enable", true); // 모듈 활성화 여부
    g_promotionRewardsCheckInterval = sConfigMgr->GetOption<uint32>("PromotionRewards.CheckInterval", 3600000); // 밀리초 단위(1시간)
    g_promotionRewardsGold = sConfigMgr->GetOption<uint32>("PromotionRewards.RewardGold", 100); // 코퍼단위
    g_promotionRewardsItemId = sConfigMgr->GetOption<uint32>("PromotionRewards.RewardItemID", 38082); // 아이템 ID 22칸 가방
    g_promotionRewardsItemQuantity = sConfigMgr->GetOption<uint32>("PromotionRewards.RewardItemQuantity", 1); // 아이템 수량
    g_promotionRewardsMailSubject = sConfigMgr->GetOption<std::string>("PromotionRewards.MailSubject", "홍보 활동 보상이 도착했습니다!"); // 메일 제목
    g_promotionRewardsMailBody = sConfigMgr->GetOption<std::string>("PromotionRewards.MailBody", "참여해주셔서 감사합니다! 다음은 승인된 홍보 활동에 대한 보상입니다."); // 메일 본문

    LOG_INFO("module", "[홍보 보상] 모듈 설정 로드 완료.");
}

class mod_promotion_rewards_world : public WorldScript
{
public:
    mod_promotion_rewards_world() : WorldScript("mod_promotion_rewards_world") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        LoadPromotionRewardsConfig();
    }

    void OnUpdate(uint32 diff) override
    {
        if (!g_promotionRewardsEnabled)
            return;

        if (g_timeUntilNextCheck <= diff)
        {
            // DB 확인 로직 실행
            LOG_INFO("module", "[홍보 보상] 데이터베이스에서 보상 대상을 확인합니다...");

            // DB 쿼리 실행
            QueryResult result = LoginDatabase.Query("SELECT id, character_name FROM promotions WHERE status = 'approved' AND reward_sent = 0");

            if (result)
            {
                do
                {
                    Field* fields = result->Fetch();
                    uint32 promotionId = fields[0].Get<uint32>();
                    std::string characterName = fields[1].Get<std::string>();

                    // characterName을 소문자로 변환
                    std::transform(characterName.begin(), characterName.end(), characterName.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                    LOG_INFO("module", "[홍보 보상] 처리 대상 발견: ID={}, 캐릭터={}", promotionId, characterName);

                    ObjectGuid playerGuidObj = sCharacterCache->GetCharacterGuidByName(characterName); // ObjectMgr -> CharacterCache
                    if (!playerGuidObj)
                    {
                        LOG_ERROR("module", "[홍보 보상] 캐릭터를 찾을 수 없습니다: {}", characterName);
                        continue;
                    }

                    WorldSession* session = sWorldSessionMgr->FindSession(playerGuidObj.GetCounter());
                    Player* player = nullptr;
                    if (session)
                        player = session->GetPlayer();

                    // 보상 지급
                    if (player && player->GetSession())
                    {
                        // 플레이어가 온라인일 경우
                        player->ModifyMoney(g_promotionRewardsGold); // AddMoney -> ModifyMoney
                        if (g_promotionRewardsItemId > 0)
                        {
                            player->AddItem(g_promotionRewardsItemId, g_promotionRewardsItemQuantity);
                        }
                        ChatHandler(player->GetSession()).SendSysMessage("|cff4CFF00[홍보보상]|r 홍보 활동에 대한 보상이 지급되었습니다!");
                        LOG_INFO("module", "[홍보 보상] 온라인 플레이어 {}에게 보상 지급 완료.", characterName);
                    }
                    else
                    {
                        // 플레이어가 오프라인일 경우 (우편 발송)
                        MailSender sender(MAIL_CREATURE, 0, MAIL_STATIONERY_GM); // MailSender 인자 수정
                        MailDraft draft(g_promotionRewardsMailSubject, g_promotionRewardsMailBody);
                        if (g_promotionRewardsGold > 0)
                        {
                            draft.AddMoney(g_promotionRewardsGold);
                        }
                        if (g_promotionRewardsItemId > 0)
                        {
                            if (Item* item = Item::CreateItem(g_promotionRewardsItemId, g_promotionRewardsItemQuantity))
                            {
                                draft.AddItem(item);
                            }
                        }
                        // MailReceiver 객체를 명시적으로 생성
                        MailReceiver receiver(playerGuidObj.GetCounter());
                        // CharacterDatabaseTransaction 객체를 명시적으로 생성
                        CharacterDatabaseTransaction transaction = CharacterDatabase.BeginTransaction();

                        // SendMailTo 호출 시 명시적으로 생성된 객체 사용
                        draft.SendMailTo(transaction, receiver, sender);
                        LOG_INFO("module", "[홍보 보상] 오프라인 플레이어 {}에게 우편으로 보상 발송 완료.", characterName);
                    } // 온라인/오프라인 처리 로직의 닫는 괄호

                    // DB 상태 업데이트 (온라인/오프라인 여부와 관계없이 항상 실행)
                    LoginDatabase.Execute("UPDATE promotions SET reward_sent = 1, processed_at = NOW() WHERE id = {}", promotionId);
				}while (result->NextRow());
            }
            else
            {
                LOG_INFO("module", "[홍보 보상] 처리할 새로운 보상 항목이 없습니다.");
            }

            g_timeUntilNextCheck = g_promotionRewardsCheckInterval;
        }
        else
        {
            g_timeUntilNextCheck -= diff;
        }
	}
};

void Addmod_promotion_rewardsScripts()
{
    new mod_promotion_rewards_world();
}
