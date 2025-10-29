#include "EnemyMove.h"
#include "GamePlay.h"
#include "WeakEnemy.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(WeakEnemy* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(32.0f)
	, mAttackTime(1.0f)
	, mAttackTimer(100.0f)
	, mHitTime(0.5f)
	, mHitTimer(0.0f)
{
	mJumpSpeed = -600.0f;
	// ターゲット指定(EnemyActor側から指定してもいい)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	// 自由落下への遷移判定を最初に行う
	if (mVelocityY > 0.0f &&
		mEnemy->getEnemyState() != WeakEnemy::E_jump &&
		mEnemy->getEnemyState() != WeakEnemy::E_hit)
	{
		mEnemy->changeState(WeakEnemy::E_jump);
	}

	// 重力(垂直移動)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	mAttackTimer += GetFrameTime();
	// ステートによって処理が変わる
	switch (mEnemy->getEnemyState())
	{
	case WeakEnemy::E_walk:
	{
		// 自身の位置に対するプレイヤーの位置の方向を取得(自身正なら-1、負なら1に進む)
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//水平移動の速度を設定
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		
		// もし射程内で
		if (withinRange()) {
			// 攻撃インターバルを終えていたら
			if (mAttackTime < mAttackTimer) {
				// attack状態へ
				mEnemy->changeState(WeakEnemy::E_attack);
				mVelocityX = 0.0f;	// attack時に速度を0にしてみる
				mAttackTimer = 0.0f;
			}
		}
		break;
	}
	case WeakEnemy::E_jump:
	{
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//水平移動の速度を設定
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		break;
	}
	case WeakEnemy::E_attack:
	{
		// 射程外なら探索
		if (!withinRange()) {
			mEnemy->changeState(WeakEnemy::E_walk);
		}
		// 射程内で一定時間たったら
		else if (mAttackTime < mAttackTimer) {
			// 自身の位置に対するプレイヤーの位置の方向を設定
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
			mEnemy->changeState(WeakEnemy::E_attack);
			mAttackTimer = 0.0f;
		}
		break;
	}
	case WeakEnemy::E_hit:
	{
		mHitTimer += GetFrameTime();
		if (mHitTime < mHitTimer) {
			mHitTimer = 0.0f;
			mEnemy->changeState(WeakEnemy::E_walk);
		}
	}
	}

	//速度を位置に反映
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();
	mOwner->setPosition(ownerPos);
	// Enemyの移動が完全に終了した後の位置を基に逐次描画するためのRectangle位置明示
	mOwner->computeRectangle();
}

void EnemyMove::fixFloorCol()
{
	// Y方向の速度を初期化
	mVelocityY = 0.0f;
	// 空中からの解消の場合はStateを変更する
	if (mEnemy->getEnemyState() == WeakEnemy::E_jump) {
		mEnemy->changeState(WeakEnemy::E_walk);
	}
}

bool EnemyMove::withinRange()
{
	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	if (abs(ownerPos.x - targetPos.x) < mAttackRange &&
		abs(ownerPos.y - targetPos.y) < mAttackRange) {
		return true;
	}
	else {
		return false;
	}
}
