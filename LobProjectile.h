#pragma once
//===============================================
// LobProjectile.h
//  放物線で飛ぶ投擲弾。寿命かヒットで消滅。
//===============================================
#include "Actor.h"
#include "DamageTags.h"
#include <cmath>

class GamePlay;

class LobProjectileActor : public Actor {
public:
    // seq    : 所属シーケンス
    // pos    : 初期位置（投擲位置）
    // forward: 向き（+1 右 / -1 左）
    LobProjectileActor(GamePlay* seq, Vector2 pos, int forward);

    void update() override;          // 物理（重力）と当たり判定
    void computeRectangle() override;// 当たり矩形（今回は16x16）

    DamageTag tag = DamageTag::ProjectileLob; // 参照用（未使用でも可）

private:
    GamePlay* mGP   = nullptr;
    Vector2   mVelocity  = {};    // 速度：X は向き依存、Y は上向き初速
    float     mLife = 4.0f;  // 寿命（秒）
    class SpriteComponent* mSprite = nullptr; // ★ 回転更新のため保持
    static constexpr float kW = 16.0f; // ★ 当たり/見た目の幅
    static constexpr float kH = 16.0f; // ★ 当たり/見た目の高さ
};
