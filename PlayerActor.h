#pragma once
#include "Actor.h"
#include <unordered_map>
#include "PlayerMove.h"
#include "PlayerState.h"

class PlayerState;
enum class Type;

/// <summary>
/// PlayerActorクラス
/// </summary>
class PlayerActor :
    public Actor
{
public:
    PlayerActor(class Sequence* sequence);
    ~PlayerActor();

    void input() override;
    void update() override;

    class PlayerMove* getPlayerMove() const { return mPlayerMove; }
    class AttackComponent* getAttackComp() const { return mAttackComp; }
    PlayerState* getPlayerState() const { return mPlayerState; }
    class AnimSpriteComponent* getAnimSpriteComp() const { return mAnimsc; }
    class HpComponent* getHpComp() { return mHpComp; }
    
    void computeRectangle() override;
    void changeState(PlayerState::Type type);

    void onDead() { mIsDead = true; }
    bool isDead() const { return mIsDead; }
private:
    // ステージとの当たり判定
    void fixCollision();
    void stageCollision(const struct Rectangle &stageRec);

    PlayerState* mPlayerState;
    std::unordered_map<PlayerState::Type, PlayerState*> mPlayerStates;
    class PlayerMove* mPlayerMove;
    class AnimSpriteComponent* mAnimsc;
    class AttackComponent* mAttackComp;
    class HpComponent* mHpComp;
    
    bool mIsDead;
};

// 挙動
// AD : 移動
// Space : ジャンプ
// Shift + 移動 : ダッシュ
// S : ガード(移動,ジャンプ不可)
// W : 攻撃
