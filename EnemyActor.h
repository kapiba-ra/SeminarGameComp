#pragma once
#include "Actor.h"

/// <summary>
/// 敵アクター
/// 必要に応じてまた継承する
/// </summary>
class EnemyActor :
    public Actor
{
public:
    enum class Type
    {
        Weak,
        Boss
    };
    virtual ~EnemyActor();
    class HpComponent* getHpComp() { return mHpComp; }
    Type getType() const { return mType; }

protected:
    EnemyActor(class Sequence* sequence, Type type);
    class HpComponent* mHpComp;
    class AnimSpriteComponent* mAnimsc;
private:
    Type mType;
};

