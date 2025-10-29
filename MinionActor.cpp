//===============================================
// MinionActor.cpp
//===============================================
#include "MinionActor.h"
#include "Boss.h"
#include "GamePlay.h"
#include "HpComponent.h"  // SetMaxHp() を呼ぶため

MinionActor::MinionActor(GamePlay* seq, Boss* ownerBoss, float life, float heal)
    : EnemyActor(seq, Type::Weak)
    , mBoss(ownerBoss)
    , mLife(life)
    , mHealAmt(heal)
{
    // 召喚ミニオンはHP低め（必要なら見た目・攻撃など追加）
    mHpComp->SetMaxHp(10.0f);
}

void MinionActor::update() {
    EnemyActor::update();

    // タイマが切れたら自滅 → ボス回復
    mLife -= GetFrameTime();
    if (mLife <= 0.0f) {
        if (mBoss) mBoss->Heal(mHealAmt);
        setState(Actor::Edead);
    }
}
