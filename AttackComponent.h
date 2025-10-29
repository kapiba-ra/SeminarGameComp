#pragma once
#include "Component.h"
#include "AttackInfo.h"

struct KnockbackInfo
{
    class Actor* target;    // ノックバックで移動する対称
    Vector2 velocity;       // ノックバック方向と速度
    float timer;            // ノックバック時間
};

/// <summary>
/// 攻撃を行う(HPを減らす,ノックバック付与,効果音鳴らす)機能を付与するcomponent
/// 外部からAttackInfoを受け取り、それに基づいた処理をする
/// </summary>
class AttackComponent :
    public Component
{
public:
    AttackComponent(class Actor* owner);

    void update() override;
    // 攻撃する際に呼び出す,攻撃中にも呼べる
    void startAttack(AttackInfo* info);

private:
    void processAttackEnemy();
    void processAttackPlayer();
    void processAttackStageObj();
    float kbNormCalculator(int kbType, float damage); // 2025/10/21追加 ノックバックの初速度計算
    
    float mTimer;
    // 現在の攻撃情報
    AttackInfo* mCurInfo;
    bool mActive;

    std::vector<KnockbackInfo> mKnockbackTargets;
    std::vector<Actor*> mHitActors;
};

