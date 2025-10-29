#pragma once
#include "UIScreen.h"
#include <raylib.h>
#include <vector>

struct DamageNumber {
    bool active;
    int amount;
    Vector2 position;
    float timer;
};

/// <summary>
/// 攻撃ヒット時に表示するダメージ量のui
/// </summary>
class DamageUI :
    public UIScreen
{
public:
    DamageUI(class GamePlay* sequence);

    void update() override;
    void draw() override;

    // ダメージ量と発生位置を受け取る
    void addDamage(int amount, Vector2 worldPos);

private:
    class GamePlay* mGamePlaySeq;   // キャスト面倒
    std::vector<DamageNumber> mDamagePool;
    int mVelocityY;  // ダメージ表記が上昇する速度
};

