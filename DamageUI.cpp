#include "DamageUI.h"
#include "CameraSystem.h"
#include "GamePlay.h"

DamageUI::DamageUI(class GamePlay* sequence)
    : UIScreen(sequence)
    , mGamePlaySeq(sequence)
    , mVelocityY(-1)
{
    mDamagePool.resize(10); // 最大10個まで表示
}

void DamageUI::update()
{
    float dt = GetFrameTime();
    for (auto& dn : mDamagePool) {
        if (!dn.active) continue;
        dn.timer -= dt;
        dn.position.y += mVelocityY;
        if (dn.timer <= 0.0f) dn.active = false;
    }
}

void DamageUI::draw()
{
    for (auto& dn : mDamagePool) {
        if (!dn.active) continue;
        DrawText(std::to_string(dn.amount).c_str(),
            (int)dn.position.x, (int)dn.position.y, 15, RED);
    }
}

void DamageUI::addDamage(int amount, Vector2 worldPos)
{
    for (auto& dn : mDamagePool) {
        if (!dn.active) {
            dn.active = true;
            dn.amount = amount;
            // スクリーン座標に変換
            dn.position = GetWorldToScreen2D(worldPos, mGamePlaySeq->getCamera());
            dn.timer = 1.0f;
            break;
        }
    }
}
