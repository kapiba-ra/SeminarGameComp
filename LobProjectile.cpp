//===============================================
// LobProjectile.cpp
//===============================================
#include "LobProjectile.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"     // TakeDamage() 呼び出しのために必要
#include "SpriteComponent.h"
#include <cmath>


LobProjectileActor::LobProjectileActor(GamePlay* seq, Vector2 pos, int forward)
    : Actor(seq, Actor::Eweapon)
    , mGP(seq)
    , mLife(4.0f)
{
    // 位置と初速
    setPosition(pos);
    mVelocity      = { 250.0f * (float)forward, -380.0f };  // 横に 250、上向きに 380
    mForward  = forward;

    // ★ 見た目：存在する矢テクスチャ（testArrow.png を優先）
    Texture2D* tex = mGP->getTexture("Assets/testArrow.png");
    if (!tex) tex = mGP->getTexture("Assets/arrow.png");
    if (tex) {
        mSprite = new SpriteComponent(this);
        mSprite->setTexture(*tex);
        // 回転APIが無いのでここは何もしない
    }
}

void LobProjectileActor::update() {
    const float dt = GetFrameTime();

    // 重力＆移動
    mVelocity.y += 980.0f * GetFrameTime();
    mPosition.x += mVelocity.x * GetFrameTime();
    mPosition.y += mVelocity.y * GetFrameTime();

    // ★ 当たりは移動後に確定（見た目と一致させる）
    computeRectangle();

    // 画面外・寿命で消滅（既存のまま）
    mLife -= dt;
    if (mLife <= 0.0f) {
        setState(Actor::Edead);
        return;
    }

    // プレイヤーにヒットしたらダメージを与えて消滅
    auto* player = mGP->getPlayer();
    if (CheckCollisionRecs(player->getRectangle(), mRectangle)) {
        player->getHpComp()->TakeDamage(20.0f);
        setState(Actor::Edead);
    }
}

void LobProjectileActor::computeRectangle() {
    // 16x16 の簡易当たり
    mRectangle = { mPosition.x - kW * 0.5f, mPosition.y - kH * 0.5f, kW, kH };

}
