//===============================================
// BossActor.cpp
//===============================================
#include "BossActor.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"

BossActor::BossActor(GamePlay* seq, Vector2 spawnPos)
    : Actor(seq, Actor::Eenemy)
    , mGP(seq)
{
    setPosition(spawnPos);
    computeRectangle();
}

void BossActor::update() {
    // HP が尽きていれば消滅
    if (mHp <= 0.0f) {
        setState(Actor::Edead);
        return;
    }

    moveTowardPlayer();
    computeRectangle();
    tryContactDamage();
}

void BossActor::takeDamage(float dmg) {
    mHp -= dmg;
    if (mHp <= 0.0f) {
        mHp = 0.0f;
        setState(Actor::Edead);
    }
}

void BossActor::computeRectangle() {
    // mPosition を中心とした当たり矩形
    mRectangle = { mPosition.x - mHalfW, mPosition.y - mHalfH, mHalfW * 2.0f, mHalfH * 2.0f };
}

void BossActor::moveTowardPlayer() {
    if (!mGP) return;
    auto* player = mGP->getPlayer();
    if (!player) return;

    const Vector2 p = player->getPosition();
    Vector2 delta { p.x - mPosition.x, p.y - mPosition.y };

    // とりあえず X 方向のみ寄っていく（跳ね回り防止のため簡易化）
    if (delta.x > 1.0f)      mPosition.x += mMoveSpeedPerTick;
    else if (delta.x < -1.0f) mPosition.x -= mMoveSpeedPerTick;

    // 余裕があれば Y も追尾したい場合は下を解放
    // if (delta.y > 1.0f)       mPosition.y += mMoveSpeedPerTick * 0.6f;
    // else if (delta.y < -1.0f) mPosition.y -= mMoveSpeedPerTick * 0.6f;
}

void BossActor::tryContactDamage() {
    if (!mGP) return;
    auto* player = mGP->getPlayer();
    if (!player) return;

    if (mContactTimer > 0) {
        --mContactTimer;
        return;
    }

    // プレイヤー矩形と接触したらダメージ
    if (CheckCollisionRecs(player->getRectangle(), mRectangle)) {
        if (auto* hp = player->getHpComp()) {
            hp->TakeDamage(mContactDamage);
        }
        mContactTimer = mContactCooldownFrames;
    }
}
