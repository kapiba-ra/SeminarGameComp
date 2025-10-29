#pragma once
#include "MoveComponent.h"

/// <summary>
/// �G�̈ړ��������N���X
/// </summary>
class EnemyMove :
    public MoveComponent
{
public:
    EnemyMove(class WeakEnemy* owner);

    void update()override;
    void fixFloorCol();
    void setAttackRange(float range) { mAttackRange = range; }
    void setAttackTime(float time) { mAttackTime = time; }

private:
    bool withinRange();        // �˒����ł��邩�ǂ����𒲂ׂ�
    class WeakEnemy* mEnemy;   // mOwner�Ɠ���(cast�ȗ��p)
    class Actor* mTarget;
    float mAttackRange;
    float mAttackTime;
    float mAttackTimer;
    float mHitTime;
    float mHitTimer;
};

