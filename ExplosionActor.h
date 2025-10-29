#pragma once
#include "Actor.h"

/// <summary>
/// �����A�N�^�[
/// �G�t�F�N�g�݂����ȃC���[�W�ň���
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

