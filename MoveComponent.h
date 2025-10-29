#pragma once
#include "Component.h"

/// <summary>
/// PlayerMove,EnemyMove�̊��
/// </summary>
class MoveComponent :
    public Component
{
public:
    MoveComponent(class Actor* owner);
    void update() override {} ;

    // �ȉ��ꉞ�p��
    float getMoveSpeed() const { return mMoveSpeed; }
    float getJumpSpeed() const { return mJumpSpeed; }
    float getVelocityX() const { return mVelocityX; }
    float getVelocityY() const { return mVelocityY; }
    void setMoveSpped(float speed) { mMoveSpeed = speed; }
    void setJumpSpped(float speed) { mJumpSpeed = speed; }
    void setVelocityX(float velocity) { mVelocityX = velocity; }
    void setVelocityY(float velocity) { mVelocityY = velocity; }

protected:
    float mGravity;     // �d��
    float mMoveSpeed;   // �������̈ړ����x
    float mVelocityX;   // �������̑��x
    float mJumpSpeed;   // �W�����v�̏����x
    float mVelocityY;   // �c�����̑��x
};

