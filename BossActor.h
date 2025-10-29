#pragma once
//===============================================
// BossActor.h
//===============================================
#include "Actor.h"
#include <raylib.h>   // Rectangle, Vector2

class GamePlay;
class PlayerActor;
class HpComponent;

// シンプルなボス。
// - 体力を持つ（takeDamage で減少）
// - プレイヤーに向かってゆっくり接近
// - 接触ダメージあり（クールダウン付き）
// - mRectangle は当たり判定（描画サイズに合わせて変更可）
class BossActor : public Actor {
public:
    BossActor(GamePlay* seq, Vector2 spawnPos);
    ~BossActor() override = default;

    void update() override;

    // 外部（弾やビーム）から被弾させるための API
    void takeDamage(float dmg);

    // 参照用
    const Rectangle& getRectangle() const override { return mRectangle; }
    float getHp() const { return mHp; }

private:
    void computeRectangle() override;          // mPosition から mRectangle を更新
    void moveTowardPlayer();          // 単純な追尾 AI
    void tryContactDamage();          // 接触ダメージ（CD あり）

private:
    GamePlay* mGP{};
    Rectangle mRectangle{};

    // 調整パラメータ（お好みで変更）
    float mHp{ 1000.0f };
    float mMoveSpeedPerTick{ 1.5f };   // 1 フレームあたりの移動量（例: 60fps ≒ 90px/s）
    float mContactDamage{ 12.0f };
    int   mContactCooldownFrames{ 36 }; // 60fps を想定して 0.6 秒
    int   mContactTimer{ 0 };           // クールダウン用カウンタ

    // 当たり判定サイズ（中心基準）。スプライトに合わせて調整。
    float mHalfW{ 44.0f };
    float mHalfH{ 44.0f };
};
