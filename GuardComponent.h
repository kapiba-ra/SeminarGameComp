#pragma once
//===============================================
// GuardComponent.h
//  ボス専用の“ガード”ゲージ（岩/アーマー）を管理するコンポーネント。
//  ・HPとは独立したゲージ
//  ・ガードが残っている間は HP への実ダメージを 10% に抑える（Boss側で併用）
//  ・ダメージ種別（DamageTag）に応じてガードの減り方が変化
//  ・ガードブレイク直後の“ガード無敵”時間を持ち、多段HITで一瞬で消し飛ぶのを防止
//===============================================
#include "Component.h"
#include "DamageTags.h"

class GuardComponent : public Component {
public:
    // owner  : このコンポーネントを保持する Actor
    // maxGuard : ガード最大値
    // breakInvincible : ガードブレイク直後のガード無敵秒数
    GuardComponent(class Actor* owner, float maxGuard = 100.0f, float breakInvincible = 0.0f);

    void update() override;                 // 毎フレーム：ガード無敵タイマの更新
    bool HasGuard() const { return mCurGuard > 0.0f; }     // ガードが残っているか
    float GetRatio() const { return mCurGuard / mMaxGuard; } // 0..1 表示用
    float GetCur() const { return mCurGuard; }
    float GetMax() const { return mMaxGuard; }

    // ガードへダメージ適用。戻り値は実際に“削れたガード量”
    float TakeGuardDamage(float dmg, DamageTag tag);

    // 回復系
    void RechargeFull();            // 全回復（HP 1/2・1/4到達時のリチャージで使用）
    void RechargePercent(float r);  // 割合回復（0..1）

    void SetMax(float max) { mMaxGuard = max; if (mCurGuard > mMaxGuard) mCurGuard = mMaxGuard; }

private:
    float mCurGuard;              // 現在ガード値
    float mMaxGuard;              // 最大ガード値

    float mBreakInvincDuration;   // ガードブレイク直後のガード無敵 持続秒
    float mBreakInvincTimer;      // 同タイマ
};
