#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player�̈ړ����i��
/// Player��State��ύX�����������
/// </summary>
class PlayerMove :
    public MoveComponent
{
public:
    PlayerMove(class PlayerActor* owner);
    void input()override;
    void update()override;
    void fixFloorCol();
    void fixCeilingCol();

    // �W�����v����u�ԂɌĂ�
    void jump();

    void setMultiplier(float buff, float duration);

private:
    class PlayerActor* mPlayer; // mOwner�Ɠ���(�^���Ⴄ)
    float mMultiplier;  // �o�t���|����ۂɎg�p
    float mBuffDuration;
};
