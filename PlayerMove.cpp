#include "PlayerMove.h"
#include <raylib.h>
#include <string>

#include "PlayerActor.h"

PlayerMove::PlayerMove(PlayerActor* owner)
	: MoveComponent(owner)
	, mPlayer(owner)
	, mMultiplier(1.0f)
	, mBuffDuration(-1.0f)
{
}

void PlayerMove::input()
{
	// 自由落下の遷移はここで行う
	if (mVelocityY > 0.0f && 
		mPlayer->getPlayerState()->getType() != PlayerState::Type::Jump &&
		mPlayer->getPlayerState()->getType() != PlayerState::Type::NormalAttack
		) {
		mPlayer->changeState(PlayerState::Type::Jump);
	}

	if (IsKeyDown(KEY_D)) {
		mPlayer->setForward(1);
	}
	else if(IsKeyDown(KEY_A)) {
		mPlayer->setForward(-1);
	}
}

void PlayerMove::update()
{
	// 重力
	if (mVelocityY < 600.0f) { // 600.0fは加速の上限
		mVelocityY += mGravity;
	}
	// 位置をキャッシュ
	Vector2 pos = mOwner->getPosition();

	// 速度を設定
	pos.x += mVelocityX * mPlayer->getForward() / GetFPS() * mMultiplier;
	pos.y += mVelocityY / GetFPS();
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される
	mOwner->setPosition(pos);
	mOwner->computeRectangle();

	// 横方向速度を初期化,回避中,回避攻撃中なら変更しない
	if (mPlayer->getPlayerState()->getType() != PlayerState::Type::Dodge &&
		mPlayer->getPlayerState()->getType() != PlayerState::Type::DodgeAttack) {
		mVelocityX = 0.0f;
	}

	if (mBuffDuration > 0) {
		mBuffDuration -= GetFrameTime();
		if (mBuffDuration <= 0.0f) mMultiplier = 1.0f;
	}
}

// GamePlayで呼ぶ。床との衝突解消に使われる。
// 重力は常に働いているので地上にいるとき常に呼ばれる
void PlayerMove::fixFloorCol()
{
	// Y方向の速度を初期化
	mVelocityY = 0.0f;
	// 空中からの解消の場合はStateを変更する
	if (mPlayer->getPlayerState()->getType() == PlayerState::Type::Jump) {
		mPlayer->changeState(PlayerState::Type::Idle);
	}
}

// 天井との衝突
void PlayerMove::fixCeilingCol()
{
	mVelocityY = 0.0f;
}

void PlayerMove::jump()
{
	mVelocityY = mJumpSpeed;
}

void PlayerMove::setMultiplier(float buff, float duration)
{
	mMultiplier = buff;
	mBuffDuration = duration;
}
