#pragma once
#include "Actor.h"

/// <summary>
/// アイテムの基底クラス
/// プレイヤーに当たると影響を及ぼす
/// 回復とスピードアップの二つのアイテムがある
/// </summary>
class ItemActor :
    public Actor
{
public:
    ItemActor(class Sequence* sequence);
    virtual ~ItemActor() {};

    // プレイヤーとの衝突判定を行っています
    void update() override;
    // 当たり判定の更新
    void computeRectangle() override;
    // プレイヤーがアイテム触れた時に呼び出される関数
    // 純粋仮想関数にしたので派生クラスで必ず実装しなくてはならない)
    virtual void onAcquired() = 0;

protected:
    class SpriteComponent* mSpriteComp;
};

/// <summary>
/// 移動速度上昇アイテム
/// </summary>
class SpeedUpItem
    : public ItemActor
{
public:
    SpeedUpItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mDuration;        // 効果時間
    float mBuffMultiplier;  // 倍率でバフを掛ける
};

// TODO: プログラム課題Ex 回復アイテムの実装
// 詳しい説明は「プログラミング体験Ex説明.docx」に乗せています
// 1.ItemActor.h, ItemActor.cppに回復アイテムを実装する
// 2.WeekEnemy.cpp雑魚敵が死亡したときに確率で回復アイテムを落とすようにする
// という流れです
// SpeedUpItemを参考に作ってみて下さい

/// <summary>
/// 回復アイテム
/// </summary>
class HealingItem
    : public ItemActor
{
public:
    HealingItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mHealAmount;
};