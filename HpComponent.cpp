#include "HpComponent.h"
#include <iostream>
#include <raylib.h>
#include <string>
#include "GamePlay.h"
#include "Actor.h"
#include "DamageUI.h"

HpComponent::HpComponent(Actor* owner, float maxHp, float invincibleDuration)
	: Component(owner)
	, mMaxHp(maxHp)
	, mCurHp(maxHp)
	, mInvincibleDuration(invincibleDuration)
	, mInvincibleTime(0.0f) //初期値は0で無敵でないとする
{
}
// ★追加：update関数の具体的な処理を実装
void HpComponent::update()
{
	// 無敵時間が残っている場合
	if (mInvincibleTime > 0.0f)
	{
		// 経過時間（デルタタイム）をタイマーから引く
		mInvincibleTime -= GetFrameTime();
	}
}

bool HpComponent::TakeDamage(float damage)
{
	if (isInvincible()) {
		return false;
	}

	mInvincibleTime = mInvincibleDuration;
	// hpは0以下にならない
	mCurHp -= damage;
	if (mCurHp <= 0.0f) {
		mCurHp = 0.0f;
		return true;
	}
	//デバッグ用
	std::cout << "mCurHp=" << mCurHp << std::endl;

	// UIへ通知(お茶濁し実装)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	seq->getDamageUI()->addDamage((int)damage, mOwner->getPosition());

	return false;
}

void HpComponent::Recover(float recover)
{
	// hpはmMaxHpを超えない
	float diff = mMaxHp - mCurHp;
	if (diff <= recover) {
		recover = diff;
	}
	mCurHp += recover;
}

void HpComponent::AddMaxHp(float add)
{
	mMaxHp += add;
	mCurHp += add;
}