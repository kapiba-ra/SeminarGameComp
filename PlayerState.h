#pragma once
#include "Animation.h"
#include "AttackInfo.h"

class PlayerActor;

/// <summary>
/// プレイヤーの状態機械の基底
/// </summary>
class PlayerState
{
public:
    enum class Type
    {
        Idle,
        Walk,
        Jump,
        Guard,
        Dodge,
        Charge,
        NormalAttack,
        DodgeAttack,
        ChargeAttack,
        Dying
    };
    virtual ~PlayerState() {}
    virtual void enter();       // 状態に入った時に呼び出す
    virtual void exit() {}      // 状態を出る時に呼び出す
    virtual void input() {}     // 入力
    virtual void update() {}    // 更新

    Type getType() const { return mType; }
    void setAnimation(Animation anim) { mAnim = anim; }
protected:
    // コンストラクタでプレイヤーを持つ
    PlayerState(PlayerActor* player, Type type);
    PlayerActor* mPlayer;
    const Type mType;
    Animation mAnim;
    // 攻撃状態のみで使う,攻撃範囲の位置を計算する
    void computeAttackRectPos(Rectangle &rec);
};

// Idle
class Idle
    : public PlayerState
{
public:
    Idle(PlayerActor* player);
    void input() override;
    void enter() override;
private:
    double mLastPressedTimeD;
    double mLastPressedTimeA;
    double mDoubleTapWindow;     // 回避の際の2回入力の受付猶予時間
};

// 歩き
class Walk
    : public PlayerState
{
public:
    Walk(PlayerActor* player);
    void input() override;
    void update() override;
private:
    float mWalkSpeed;
};

// ジャンプ
class Jump
    : public PlayerState
{
public:
    Jump(PlayerActor* player);
    void enter() override;
    void input() override;
    void update() override;
private:
    float mHorizontalSpeed; // ジャンプ中の横移動速度
};

// ガード
class Guard
    : public PlayerState
{
public:
    Guard(PlayerActor* player);
    void input() override;
    void update() override;
    void enter() override;
    void onAttacked();
private:
    Animation mUniqueAnim;  // ガード中(攻撃を受けていないとき)のアニメーション
    bool isGuarding;    // ガード成功中
};

// 回避
class Dodge
    : public PlayerState
{
public:
    Dodge(PlayerActor* player);
    void input() override;
    void update() override;
    void enter() override;
private:
    float mDodgeTime;
    float mDodgeTimer;
    float mDodgeSpeed;  // 回避中の移動速度
};

// ため
class Charge
    : public PlayerState
{
public:
    Charge(PlayerActor* player);
    void input() override;
    void update() override;
private:
    float mChargeTimer;
    float mChargeTime;
    std::vector<Texture2D*> mChargeFrames;
    std::vector<Texture2D*> mChargeMaxFrames;
    bool mChargeMax;
};

// 通常攻撃
class NormalAttack
    : public PlayerState
{
public:
    NormalAttack(PlayerActor* player);
    void update() override;
    void enter() override;
private:
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};

// 回避攻撃
class DodgeAttack
    : public PlayerState
{
public:
    DodgeAttack(PlayerActor* player);
    void update() override;
    void enter() override;
    void exit() override;
private:
    float mHorizontalSpeed;
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};

// ため攻撃
class ChargeAttack
    : public PlayerState
{
public:
    ChargeAttack(PlayerActor* player);
    void update() override;
    void enter() override;
private:
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};

class Dying
    : public PlayerState
{
public:
    Dying(PlayerActor* player);
    void update() override;
};

// 状態
// idle
// walk
// jump
// dodge
// charge
// normalAttack
// dodgeAttack
// chargeAttack
// dying