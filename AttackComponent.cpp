#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "WeakEnemy.h"
#include "PlayerActor.h"
#include "Boss.h"
#include "StageObject.h"

#include "HpComponent.h"
#include "SoundSystem.h"

AttackComponent::AttackComponent(Actor* owner)
	: Component(owner)
	, mTimer(0.0f)
	, mActive(false)
	, mCurInfo(nullptr)
{
}

void AttackComponent::update()
{
	if (mActive) {
		/* 攻撃終了判定 */
		mTimer += GetFrameTime();
		// 攻撃時間を過ぎたら非activeに
		if (mTimer > mCurInfo->duration) {
			mActive = false;
		}

		// TODO : プログラム課題③ targetMaskをどう使うか
		// 現状の実装だと,ビットマスクとして使えていないのでバグが発生します.
		// bit演算子を活用して複数のターゲットに対応できるようにしましょう.
		// なお,Actor::TypeはEenenmy,Eplayer,EstageObject以外は使いません

		/* 修正箇所 */
		if (mCurInfo->targetMask & Actor::Type::Eenemy) {
			processAttackEnemy();
		}
		if (mCurInfo->targetMask & Actor::Type::Eplayer) {
			processAttackPlayer();
		}
		if (mCurInfo->targetMask & Actor::Type::EstageObject) {
			processAttackStageObj();
		}
		/* 修正箇所 */


	}

	//kb move
	for (int i = mKnockbackTargets.size() - 1; i >= 0; --i) {
		auto& info = mKnockbackTargets[i];

		//ターゲットが有効かチェック
		if (info.target->getState() == Actor::Edead) {
			mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
			continue;
		}
		info.timer -= GetFrameTime();

		if (info.timer <= 0.0f) {
			mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
		}
		else {
			// TODO: プログラム体験①,② 重力弄れます
			/* ------ ノックバック中の位置更新 開始------ */
			float gravity = 50.0f;
			Vector2 currentPos = info.target->getPosition();
			info.velocity.y += gravity;
			Vector2 moveAmount = Vector2Scale(info.velocity, GetFrameTime());
			Vector2 newPos = Vector2Add(currentPos, moveAmount);

			// NOTE: 以下弄らなくてよい
			info.target->setPosition(newPos);
			info.target->computeRectangle();
			/* ------ 敵のノックバック中の位置更新 終了------ */
		}
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // 現在の攻撃を設定
	mHitActors.clear();
}

void AttackComponent::processAttackEnemy()
{
	std::vector<EnemyActor*> enemies = 
		static_cast<GamePlay*>(mOwner->getSequence())->getEnemies();
	
	for (auto enemy : enemies) {
		if (enemy->getHpComp()->isInvincible()) {
			continue;
		}
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			mHitActors.push_back(enemy);
			

			// TODO: プログラム体験①,②
			/* ------ 敵のノックバックに上方向のベクトルを追加する ------ */
			
			// 攻撃者(プレイヤー)の位置と攻撃対象の位置をキャッシュ
			Vector2 attackerPos = mOwner->getPosition();
			Vector2 targetPos = enemy->getPosition();
			
			KnockbackInfo info;
			// ★KnockbackInfoのパラメータを弄ってみてください
			// ・定義はAttackComponent.hにあります
			// ・座標系...左上が(0.0),右下が(1280.720)

			// ノックバック方向,現在は横方向のみ
			Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));
			float upspeed = 2.0f;
			direction.y -= upspeed;
			direction = Vector2Normalize(direction);

			info.target = enemy;
			info.timer = 0.5f;

			// 現在は攻撃情報からノックバック速度を得ています
			// ②の課題ではダメージ量によってスケーリングする仕様にする等,工夫してみてください
			float speed = kbNormCalculator(2, mCurInfo->damage) * mCurInfo->knockBack;
			info.velocity = Vector2Scale(direction, speed);

			// mKnockbackTargetsに追加
			if (enemy->getType() != EnemyActor::Type::Boss) {
				mKnockbackTargets.push_back(info);
			}
			/* ------ 敵のノックバックに上方向のベクトルを追加する ------ */



			/* 以下,ダメージを与える処理 */
			if (auto boss = dynamic_cast<Boss*>(enemy)) {
				boss->ApplyDamage(mCurInfo->damage, mCurInfo->tag);
			}
			else {
				static_cast<WeakEnemy*>(enemy)->changeState(WeakEnemy::E_hit);
				if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
					enemy->setState(Actor::Edead);
					SoundSystem::instance().playSE("YarareSEb");
				}
			}
			mActive = false;
		}
	}
}

void AttackComponent::processAttackPlayer()
{
	PlayerActor* player = static_cast<GamePlay*>(mOwner->getSequence())->getPlayer();
	PlayerState* playerState = player->getPlayerState();

	// 無敵or回避ならダメージは与えない
	if (player->getHpComp()->isInvincible() ||
		playerState->getType() == PlayerState::Type::Dodge ||
		playerState->getType() == PlayerState::Type::DodgeAttack) {
		return;
	}

	// 当たったら
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		mHitActors.push_back(player);
		
		// ガード中なら
		if (playerState->getType() == PlayerState::Type::Guard) {
			static_cast<Guard*>(playerState)->onAttacked();
			// ダメージを食らわない
			// ダメージ半減等にするなら,returnを消してTakeDamageに倍率を掛ける
			return;
		}

		//　以下,ダメージやノックバックの処理

		Vector2 attackerPos = mOwner->getPosition();
		Vector2 targetPos = player->getPosition();
		Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

		float flyspeed = 0.2f;
		direction.y -= flyspeed;
		direction = Vector2Normalize(direction);
		KnockbackInfo info;
		info.target = player;
		info.timer = 0.2f;
		float speed = mCurInfo->knockBack;
		info.velocity = Vector2Scale(direction, speed);

		mKnockbackTargets.push_back(info);

		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}

void AttackComponent::processAttackStageObj()
{
	std::vector<StageObject*> objs =
		static_cast<GamePlay*>(mOwner->getSequence())->getStageObjs();
	for (auto obj : objs) {
		if (obj->getType() == StageObject::Type::Ebreakable) {
			int i = 1;
			if (CheckCollisionRecs(obj->getRectangle(), mCurInfo->colRect)) {
				obj->getHpComp()->TakeDamage(mCurInfo->damage);
				mActive = false;
			}
		}
	}
}

float AttackComponent::kbNormCalculator(int kbType, float damage)
{
	// ノックバックの基準値を計算する関数
	switch (kbType)
	{
	case 0: //線形
		return damage / 10.0f;
	case 1: //二次関数
		return damage * damage / 100.0f;
	case 2: //指数関数
		return powf(2.0f, ((damage - 5) / 10.0));
	default:
		return damage / 10.0f; // デフォルトは線形
	}
}

