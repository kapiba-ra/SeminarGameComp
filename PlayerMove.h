#pragma once
#include "MoveComponent.h"

/// <summary>
/// Playerの移動を司る
/// PlayerのStateを変更したりもする
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

    // ジャンプする瞬間に呼ぶ
    void jump();

    void setMultiplier(float buff, float duration);

private:
    class PlayerActor* mPlayer; // mOwnerと同じ(型が違う)
    float mMultiplier;  // バフを掛ける際に使用
    float mBuffDuration;
};
