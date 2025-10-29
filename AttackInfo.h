#pragma once
#include <raylib.h>
#include <vector>
#include "Actor.h"
#include "DamageTags.h"

struct AttackInfo
{
	float damage;		// ダメージ量
	float duration;		// 持続
	Rectangle colRect;	// 攻撃範囲
	float knockBack;	// ノックバック力

	unsigned int targetMask;   // 攻撃対象のビットマスク
	DamageTag tag = DamageTag::Normal; 
};