#pragma once
#include "Component.h"
#include <raylib.h>

enum class AttackType
{
    Normal,
    Charge,
    Dodge
};

/// <summary>
/// 武器Component基底クラス
/// 武器自身にアタッチするわけではない。ややこしいので名前変えてもいいかも
/// </summary>
class WeaponComponent :
    public Component
{
public:
    virtual ~WeaponComponent();
    virtual void update() override;
    virtual void startAttack(AttackType type = AttackType::Normal) {};
    virtual void endAttack();
    class WeaponActor* getWeapon() const { return mWeapon; }

protected:
    WeaponComponent(class Actor* owner);
    class WeaponActor* mWeapon;
};

