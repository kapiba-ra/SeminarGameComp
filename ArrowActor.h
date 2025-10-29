#pragma once
#include "WeaponActor.h"
#include "Animation.h"
#include "AttackInfo.h"

/// <summary>
/// 遠距離武器：弓
/// </summary>
class ArrowActor :
    public WeaponActor
{
public:
    ArrowActor(class Sequence* sequence, Type owner);

    void input() override;
    void update() override;

    void onStartAttack();
    void computeRectangle() override;

    void setVelocity(Vector2 v) { mVelocity = v; }
    void setLifeTime(float t) {mLifeTime = t;}
    bool isDead() const {return mDead;}

private:
    class AnimSpriteComponent* mAnimsc;
    class AttackComponent* mAttackComp;
    Animation mAnim;
    AttackInfo mAttackInfo;
    Vector2 mVelocity{0,0};
    float   mAge{0.0f};
    float   mLifeTime{3.0f};   // 秒
    bool    mDead{false};
};
