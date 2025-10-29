#pragma once
#include "Component.h"
#include <raylib.h>

/// <summary>
/// アクターにHpを持たせる
/// Component化したのは、敵・プレイヤー・破壊できるオブジェクト等にアタッチすることを想定したため
/// </summary>
class HpComponent :
    public Component
{
public:
    HpComponent(class Actor* owner, float maxHp = 1.0f, float invincibleDuration = 1.0f);

    void update() override;

    // bool型でtrueなら生存、falseなら死亡を判定。ダメージも内部で計算している
    bool TakeDamage(float damage);
    // 回復
    void Recover(float recover);

    // 0以下で
    bool IsKilled() const { return mCurHp <= 0.0f; }

    //現在無敵時間中かどうかを返す
    bool isInvincible() const { return mInvincibleTime > 0.0f; }

    // Getter,Setter
    float GetCurHp() const { return mCurHp; }
    void SetMaxHp(float maxHp) { mMaxHp = maxHp, mCurHp = maxHp; }
    // 便利系(SetterGetterでもできるけど...という奴)
    float GetHpRatio() const { return mCurHp / mMaxHp; }
    // 最大Hp増やす
    void AddMaxHp(float add);
    // 無敵時間設定
    void SetInvincibleDuration(float duration) { mInvincibleDuration = duration; }

private:
    float mCurHp;   // 現在Hp
    float mMaxHp;   // 最大Hp こっちはActor内で持たせてもいいと思います(作ってみて次第)

    float mInvincibleTime;
    float mInvincibleDuration;
};
