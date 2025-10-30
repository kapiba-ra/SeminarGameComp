#include "PlayerActor.h"
#include "EnemyActor.h"
#include "StageObject.h"
#include "PlayerMove.h"
#include "AnimSpriteComponent.h"
#include "HpComponent.h"
#include "AttackComponent.h"

#include "GamePlay.h"
#include "Stage.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence, Actor::Eplayer)
	, mIsDead(false)
{
	mPosition = Vector2{ 100.0f, 200.0f };
	mScale = 2.0f;
	//ハードコーディングしてみた(図形を解析して恐らくこの程度という目視によるハイパーパラメータ)
	float width = 21.0f * mScale;
	float height = 35.0f * mScale;
	mRectangle = {
		mPosition.x - width / 2.0f,
		mPosition.y - width / 2.0f,
		width,
		height
	};
	mAnimsc = new AnimSpriteComponent(this);

	mPlayerMove = new PlayerMove(this);
	mHpComp = new HpComponent(this, 200.0f);

	// 状態をマップに登録 状態は一気にnew,deleteする
	// 理由:状態切り替えの度にnew,deleteはフラグメンテーションが気になるからW
	mPlayerStates[PlayerState::Type::Idle] = new Idle(this);
	mPlayerStates[PlayerState::Type::Walk] = new Walk(this);
	mPlayerStates[PlayerState::Type::Jump] = new Jump(this);
	mPlayerStates[PlayerState::Type::Dodge] = new Dodge(this);
	mPlayerStates[PlayerState::Type::Guard] = new Guard(this);
	mPlayerStates[PlayerState::Type::Charge] = new Charge(this);
	mPlayerStates[PlayerState::Type::NormalAttack] = new NormalAttack(this);
	mPlayerStates[PlayerState::Type::DodgeAttack] = new DodgeAttack(this);
	mPlayerStates[PlayerState::Type::ChargeAttack] = new ChargeAttack(this);
	mPlayerStates[PlayerState::Type::Dying] = new Dying(this);

	mAttackComp = new AttackComponent(this);

	// 現在の状態を設定
	mPlayerState = mPlayerStates[PlayerState::Type::Idle];
}

PlayerActor::~PlayerActor()
{
	for (auto& state : mPlayerStates) {
		delete state.second;
	}
}

void PlayerActor::input()
{
	// 基底のinput() : Componentのinput
	Actor::input();
	mPlayerState->input();
}

void PlayerActor::update()
{
	// 基底のupdate() : Componentのupdate
	Actor::update();
	mPlayerState->update();

	if (mPlayerState->getType() != PlayerState::Type::Dying) {
		if (mHpComp->IsKilled()) {
			changeState(PlayerState::Type::Dying);
		}
	}

	fixCollision();

	// 当たり判定表示
	//DrawRectangleRec(mRectangle, WHITE);
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
	mRectangle.y = mPosition.y - mRectangle.height / 2.0f;
}

void PlayerActor::changeState(PlayerState::Type type)
{
	mPlayerState->exit();
	mPlayerState = mPlayerStates[type];
	mPlayerState->enter();
}

void PlayerActor::fixCollision()
{
	// ステージとの当たり判定
	for (auto& stageObj : static_cast<GamePlay*>(mSequence)->getStageObjs()) {
		stageCollision(stageObj->getRectangle());
	}

	// 画面左端との当たり判定
	Vector2 screenPos = GetWorldToScreen2D(mPosition, static_cast<GamePlay*>(mSequence)->getCamera());
	if (screenPos.x < 0.0f) {
		mPosition.x -= screenPos.x;
		computeRectangle();
	}
}

void PlayerActor::stageCollision(const Rectangle& stageRec)
{
	// 当たり判定
	if (CheckCollisionRecs(mRectangle, stageRec)) {
		// 衝突領域を取得
		Rectangle colRec = GetCollisionRec(mRectangle, stageRec);
		// 縦方向の衝突
		if (colRec.width >= colRec.height) {
			// 上から衝突
			if (mRectangle.y < colRec.y) {
				mPosition.y -= colRec.height;
 				mPlayerMove->fixFloorCol();
			}
			// 下から衝突
			else {
				mPosition.y += colRec.height;
				mPlayerMove->fixCeilingCol();
			}
		}
		// 横方向の衝突
		else {
			// 左から衝突
			if (mRectangle.x < colRec.x) {
				mPosition.x -= colRec.width;
			}
			// 右から衝突
			else {
				mPosition.x += colRec.width;
			}
		}
		// 矩形を再計算
		computeRectangle();
	}
}