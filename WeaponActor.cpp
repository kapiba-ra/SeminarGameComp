#include "WeaponActor.h"
#include "GamePlay.h"
#include "AnimSpriteComponent.h"

#include "EnemyActor.h"
#include "PlayerActor.h"
#include "HpComponent.h"
#include "BossActor.h"

WeaponActor::WeaponActor(Sequence* sequence, Type owner)
	: Actor(sequence, Type::Eweapon)
	, mAttackPower(10.0f)
	, mAttackMultiplier(1.0f)
{
}

WeaponActor::~WeaponActor()
{
}

void WeaponActor::onHit(Actor* target)
{
	float damageAmount = mAttackPower * mAttackMultiplier;

	if (target->getType() & Actor::Eenemy) {
        if (auto* enemy = dynamic_cast<EnemyActor*>(target)) {
            if (enemy->getHpComp()->TakeDamage(damageAmount)) {
                target->setState(Actor::Edead);
            }
        } else if (auto* boss = dynamic_cast<BossActor*>(target)) {
            boss->takeDamage(damageAmount);
            // BossActor 側でHP尽きたらEdeadに遷移します
        }
    }
    else if (target->getType() & Actor::Eplayer) {

		static_cast<PlayerActor*>(target)->getHpComp()->TakeDamage(damageAmount);
	}
}