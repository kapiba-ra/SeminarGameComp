#pragma once
#include "WeaponComponent.h"
#include <vector>

/// 遠距離武器：矢のコンポーネント(多段発射に対応)
class ArrowComponent : public WeaponComponent
{
public:
    ArrowComponent(class Actor* owner);

    void update() override;
    void startAttack(AttackType type = AttackType::Normal) override;

    // 調整用
    void setBaseSpeed(float v)   { mBaseSpeed = v; }
    void setLifeTime(float t)    { mLifeTime = t; }
    void setMuzzleOffset(float p){ mMuzzleOffset = p; }

private:
    void spawnArrow();
    void cullArrows();

    float mBaseSpeed   = 700.0f; // px/s
    float mLifeTime    = 3.0f;   // 秒
    float mMuzzleOffset= 8.0f;   // 発射口の見た目オフセット

    // 同時に複数本撃てるように保持(破棄方針はプロジェクトに合わせる)
    std::vector<class ArrowActor*> mArrows;
};

