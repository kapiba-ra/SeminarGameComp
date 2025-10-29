#pragma once
#include "Actor.h"

/// <summary>
/// 爆発アクター
/// エフェクトみたいなイメージで扱う
/// </summary>
class ExplosionActor :
    public Actor
{
public:
    ExplosionActor(class Sequence* sequence);

    void update() override;

private:
    void computeRectangle() override;
    static struct Animation mAnim;
    class AnimSpriteComponent* mAnimsc;
    class GamePlay* mGamePlay;
    float mDamageAmt;
    float mTimer;
    float mActiveTime;
    bool mActive;
};

