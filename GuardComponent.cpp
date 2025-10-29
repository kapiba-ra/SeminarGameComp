//===============================================
// GuardComponent.cpp
//===============================================
#include "GuardComponent.h"
#include "Actor.h"          // GetFrameTime() 利用のため（raylib ラッパ）

GuardComponent::GuardComponent(Actor* owner, float maxGuard, float breakInvincible)
    : Component(owner)
    , mCurGuard(maxGuard)
    , mMaxGuard(maxGuard)
    , mBreakInvincDuration(breakInvincible)
    , mBreakInvincTimer(0.0f)
{}

void GuardComponent::update() {
    // ガードブレイク直後の連続ヒットを軽減する簡単なガード無敵
    if (mBreakInvincTimer > 0.0f) {
        mBreakInvincTimer -= GetFrameTime();
        if (mBreakInvincTimer < 0.0f) mBreakInvincTimer = 0.0f;
    }
}

float GuardComponent::TakeGuardDamage(float dmg, DamageTag tag) {
    // ガードが無い or ガード無敵中なら何も削れない
    if (mCurGuard <= 0.0f || mBreakInvincTimer > 0.0f) return 0.0f;

    // ダメージ種別ごとのガード倍率（調整ポイント）
    // 仕様：通常に強く、爆発に弱い
    float mult = 1.0f;
    switch (tag) {
        case DamageTag::Explosion:     mult = 2.0f;  break; // 爆発はガードに通りやすい
        case DamageTag::MeleeHeavy:    mult = 1.0f;  break;
        case DamageTag::MeleeLight:    mult = 0.75f; break;
        case DamageTag::ProjectileLob: mult = 1.25f; break;
        case DamageTag::Beam:          mult = 1.25f; break;
        default:                       mult = 0.8f;  break; // Normal
    }

    // ガードを削る
    const float take = dmg * mult;
    mCurGuard -= take;

    // 0 を切ったらブレイク。ガード無敵を有効化
    if (mCurGuard <= 0.0f) {
        mCurGuard = 0.0f;
        mBreakInvincTimer = mBreakInvincDuration;
    }
    return take;
}

void GuardComponent::RechargeFull() {
    // ガード全回復
    mCurGuard = mMaxGuard;
}

void GuardComponent::RechargePercent(float r) {
    // 0..1 の範囲に丸めて割合回復
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    mCurGuard = mMaxGuard * r;
}
