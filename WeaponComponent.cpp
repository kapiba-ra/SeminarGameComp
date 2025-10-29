#include "WeaponComponent.h"

#include "WeaponActor.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

#include "HpComponent.h"

WeaponComponent::WeaponComponent(Actor* owner)
	: Component(owner)
	, mWeapon(nullptr)
{
}

WeaponComponent::~WeaponComponent()
{
	endAttack();
}

void WeaponComponent::update()
{
}

void WeaponComponent::endAttack()
{
	if (mWeapon) {
		mWeapon->setState(Actor::Edead);
		mWeapon = nullptr;
	}
}
