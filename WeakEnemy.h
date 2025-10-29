#pragma once
#include "EnemyActor.h"
#include "Animation.h"
#include "AttackInfo.h"
#include <unordered_map>
#include <string>

/// <summary>
/// 雑魚敵のクラス。以下の三種類存在する
/// ・近接タイプ
/// ・遠距離タイプ
/// (・ガードタイプ)
/// </summary>
class WeakEnemy :
    public EnemyActor
{
public:
    // 雑魚敵の状態
    enum EnemyState
    {
        E_walk,
        E_jump,
        E_attack,
        E_hit
    };

    void changeState(EnemyState nextState);
    class EnemyMove* getEnemyMove() { return mEnemyMove; }
    EnemyState getEnemyState() const { return mEnemyState; }

protected:
    WeakEnemy(class Sequence* sequence);
    void computeRectangle() override;
    virtual void onEnterState(EnemyState nextState);
    virtual void onExitState(EnemyState nextState);
    // ステージとの当たり判定
    void fixCollision();
    void jump();
    // AttackComponentのstartAttackを呼ぶor武器(矢とか)を出現させる等の役割
    virtual void attack() {};
    virtual Animation& getAnimation(EnemyState state) = 0;
    void computeAttackRectPos(Rectangle& rec);
    void dropItem();

    EnemyState mEnemyState;
    class EnemyMove* mEnemyMove;
};

class MeleeEnemy :
    public WeakEnemy
{
public:
    MeleeEnemy(class Sequence* sequence);
    void update() override;

private:
    void attack() override;
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }
    class AttackComponent* mAttackComp;
    AttackInfo mAttackInfo;
};

class RangedEnemy :
    public WeakEnemy
{
public:
    RangedEnemy(class Sequence* sequence);
    void update() override;

private:
    void onEnterState(EnemyState nextState) override;
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }

    std::vector<class ArrowActor*> mArrows;
};