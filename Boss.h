#pragma once
//===============================================
// Boss.h
//  最奥に鎮座するボス本体。ガード/4攻撃/召喚を実装。
//===============================================
#include "EnemyActor.h"
#include "DamageTags.h"

class GuardComponent;
class HpComponent;
class GamePlay;

class Boss : public EnemyActor {
public:
    
    HpComponent*    getHpComp() const { return mHpComp; }
    GuardComponent* getGuardComp() const { return mGuard; }
explicit Boss(GamePlay* seq);
    void update() override;

    const Rectangle& getRectangle() const override { return mRectangle; }
    void computeRectangle() override;


    // ダメージ適用（武器側から呼ばれる想定）
    void ApplyDamage(float dmg, DamageTag tag);

    // ミニオン自滅時の回復
    void Heal(float hp);

private:
    void tryGuardRecharge();
    void tryAttacks(float dt);
    void trySummon(float dt);

    // コンポーネント
    GuardComponent* mGuard = nullptr;
    HpComponent*    mHpComp = nullptr;

    // 攻撃CD
    float mMeleeLightCd = 0.0f;
    float mMeleeHeavyCd = 0.0f;
    float mLobCd        = 0.0f;
    float mBeamCd       = 0.0f;

    // 召喚
    float mSummonTimer    = 0.0f;
    float mSummonInterval = 8.0f;

    // ガード再生フラグ
    bool mDidHalfRecharge    = false;
    bool mDidQuarterRecharge = false;
};
