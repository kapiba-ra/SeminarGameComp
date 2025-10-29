#include "PlayerState.h"
#include <raylib.h>
#include "PlayerActor.h"
#include "Sequence.h"
#include "SoundSystem.h"

#include "PlayerMove.h"
#include "AnimSpriteComponent.h"
#include "AttackComponent.h"

void PlayerState::enter()
{
	mPlayer->getAnimSpriteComp()->play(&mAnim);
}

PlayerState::PlayerState(PlayerActor* player, Type type)
	: mPlayer(player)
	, mType(type)
{
}

void PlayerState::computeAttackRectPos(Rectangle& rec)
{
	if(mPlayer->getForward()>0) rec.x = mPlayer->getRectangle().x + mPlayer->getRectangle().width;
	else rec.x = mPlayer->getRectangle().x - rec.width;
	rec.y = mPlayer->getRectangle().y;
}


Idle::Idle(PlayerActor* player)
	: PlayerState(player, Type::Idle)
	, mLastPressedTimeA(0.0)
	, mLastPressedTimeD(0.0)
	, mDoubleTapWindow(0.2)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "idle", "png", 5); // "idle_0.png" から "idle_4.png" 
	mAnim.loop = true;
}

void Idle::input()
{
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		if (IsKeyDown(KEY_A) && GetTime() - mLastPressedTimeA < mDoubleTapWindow ||
			IsKeyDown(KEY_D) && GetTime() - mLastPressedTimeD < mDoubleTapWindow) {
			mPlayer->changeState(Type::Dodge);
		}
		else {
			mPlayer->changeState(Type::Walk);
		}
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}

	if (IsKeyPressed(KEY_A)) mLastPressedTimeA = GetTime();
	if (IsKeyPressed(KEY_D)) mLastPressedTimeD = GetTime();
}

void Idle::enter()
{
	PlayerState::enter();
	mPlayer->getPlayerMove()->setVelocityX(0.0f);
}

Walk::Walk(PlayerActor* player)
	: PlayerState(player, Type::Walk)
	, mWalkSpeed(500.0f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "walk", "png", 9); // "walk_0.png" から "walk_9.png"
	mAnim.loop = true;
}

void Walk::input()
{
	// Jump
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	// Idle
	else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
		mPlayer->changeState(Type::Idle);
	}
	// Guard
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}
	// Attack
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
}

void Walk::update()
{
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		mPlayer->getPlayerMove()->setVelocityX(mWalkSpeed);
	}
}

Jump::Jump(PlayerActor* player)
	: PlayerState(player, Type::Jump)
	, mHorizontalSpeed(500.0f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "jump", "png", 4); // "jump_0.png" から "jump_3.png"
	mAnim.loop = false;
	mAnim.fps = 10.0f;
}

void Jump::enter()
{
	PlayerState::enter();
	// 自由落下でないなら以下を呼び出す
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->getPlayerMove()->jump();
	}
}

void Jump::input()
{
	// Attack
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
}

void Jump::update()
{
	// 水平移動
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		mPlayer->getPlayerMove()->setVelocityX(mHorizontalSpeed);
	}
}

Guard::Guard(PlayerActor* player)
	: PlayerState(player, Type::Guard)
	, isGuarding(false)
{
	// ガード成功時のアニメーション
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "guard", "png", 6); // "guard_1.png" から "guard_6.png"
	mAnim.loop = false;
	// ガード中のアニメーション
	mUniqueAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "guard", "png", 1); // "guard_1.png" のみ
	mUniqueAnim.loop = true;
}

void Guard::input()
{
	// マウス右ボタンを離したらidleへ
	if (IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Idle);
	}
}

void Guard::update()
{
	// ガード成功中
	if (isGuarding) {
		// アニメーションが終了していたら
		if (!mPlayer->getAnimSpriteComp()->isAnimating()) {
			isGuarding = false;
			mPlayer->getAnimSpriteComp()->play(&mUniqueAnim);
		}
	}
}

void Guard::enter()
{
	mPlayer->getAnimSpriteComp()->play(&mUniqueAnim);
	isGuarding = false;
}

void Guard::onAttacked()
{
	if (isGuarding) return;
	isGuarding = true;
	// ガード成功時のアニメーションを流す
	mPlayer->getAnimSpriteComp()->play(&mAnim);
	// 音鳴らす
	SoundSystem::instance().playSE("GuardSE");
}


Dodge::Dodge(PlayerActor* player)
	: PlayerState(player, Type::Dodge)
	, mDodgeTime(0.6f)
	, mDodgeTimer(0.0f)
	, mDodgeSpeed(1000.0f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "dash", "png", 5); // "dash_0.png" から "dash_4.png"
	mAnim.loop = false;
	mAnim.fps = 10.0f;
}

void Dodge::input()
{
	// Jump
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
}

void Dodge::enter()
{
	PlayerState::enter();
	mDodgeSpeed = 1500.0f;
	mPlayer->getPlayerMove()->setVelocityX(mDodgeSpeed);
}

void Dodge::update()
{
	mDodgeTimer += GetFrameTime();
	mDodgeSpeed /= 1.1f;	// 減衰
	mPlayer->getPlayerMove()->setVelocityX(mDodgeSpeed);

	// idleへ
	if (mDodgeTimer >= mDodgeTime) {
		mPlayer->changeState(Type::Idle);
		mDodgeTimer = 0.0f;
	}
	// DodgeAttackへ
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::DodgeAttack);
	}
}

Charge::Charge(PlayerActor* player)
	: PlayerState(player, Type::Charge)
	, mChargeTimer(0.0f)
	, mChargeTime(0.2f)
	, mChargeMax(false)
{
	mChargeFrames = mPlayer->getSequence()->getAnimationFrames("player", "charge", "png", 6);
	mChargeMaxFrames = mPlayer->getSequence()->getAnimationFrames("player", "chargeMax", "png", 6);
	mAnim.frames = mChargeFrames;
	mAnim.loop = true;
}

void Charge::input()
{
	mChargeTimer += GetFrameTime();

	if (!mChargeMax) {
		if (mChargeTimer > mChargeTime) {
			mChargeMax = true;
			mAnim.frames = mChargeMaxFrames;
		}
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		if (mChargeTimer > mChargeTime) {
			mPlayer->changeState(Type::ChargeAttack);
		}
		else {
			mPlayer->changeState(Type::Idle);
		}
		mChargeTimer = 0.0f;
		mChargeMax = false;
		mAnim.frames = mChargeFrames;
	}
}

void Charge::update()
{
	float velX = mPlayer->getPlayerMove()->getVelocityX();
	mPlayer->getPlayerMove()->setVelocityX(velX / 2);
}

NormalAttack::NormalAttack(PlayerActor* player)
	: PlayerState(player, Type::NormalAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.3f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "attack", "png", 6); // "attack_0.png" から "attack_5.png"
	mAnim.loop = false;
	
	mAttackInfo.damage = 10.0f;
	mAttackInfo.duration = mAttackTime;
	mAttackInfo.colRect.width = 64.0f;
	mAttackInfo.colRect.height = 64.0f;
	computeAttackRectPos(mAttackInfo.colRect);
	mAttackInfo.knockBack = 600.0f;
	mAttackInfo.tag = DamageTag::MeleeLight;   // ★ 追加：軽攻撃

	// TODO: プログラム体験③ ターゲットレイヤー変更
	mAttackInfo.targetMask = Actor::Type::Eenemy | Actor::Type::EstageObject;
}

void NormalAttack::update()
{
	computeAttackRectPos(mAttackInfo.colRect);
	// 当たり判定表示
	//DrawRectangleRec(mAttackInfo.colRect, LIGHTGRAY);
	
	mAttackTimer += GetFrameTime();
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void NormalAttack::enter()
{
	PlayerState::enter();
	computeAttackRectPos(mAttackInfo.colRect);
	mPlayer->getAttackComp()->startAttack(&mAttackInfo);

	SoundSystem::instance().playSE("NormalAtkSE");
}

DodgeAttack::DodgeAttack(PlayerActor* player)
	: PlayerState(player, Type::DodgeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
	, mHorizontalSpeed(0.0f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "DA", "png", 10); // "DA_0.png" から "DA_9.png"
	mAnim.loop = false;

	mAttackInfo.damage = 9.0f;
	mAttackInfo.duration = mAttackTime;
	mAttackInfo.colRect.width = 64.0f;
	mAttackInfo.colRect.height = 64.0f;
	computeAttackRectPos(mAttackInfo.colRect);
	mAttackInfo.knockBack = 600.0f;

	// TODO: プログラム体験③ ターゲットレイヤー変更
	mAttackInfo.targetMask = Actor::Type::Eenemy | Actor::Type::EstageObject;
	//mAttackInfo.targetMask = Actor::Type::Eenemy;
}

void DodgeAttack::update()
{
	computeAttackRectPos(mAttackInfo.colRect);
	// 当たり判定表示
	//DrawRectangleRec(mAttackInfo.colRect, LIGHTGRAY);

	mHorizontalSpeed /= 1.1f;
	mPlayer->getPlayerMove()->setVelocityX(mHorizontalSpeed);
	
	mAttackTimer += GetFrameTime();
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void DodgeAttack::enter()
{
	PlayerState::enter();
	mHorizontalSpeed = mPlayer->getPlayerMove()->getVelocityX();
	computeAttackRectPos(mAttackInfo.colRect);
	mPlayer->getAttackComp()->startAttack(&mAttackInfo);

	SoundSystem::instance().playSE("DodgeAtkSE");
}

void DodgeAttack::exit()
{
}

ChargeAttack::ChargeAttack(PlayerActor* player)
	: PlayerState(player, Type::ChargeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "chargeAttack", "png", 3);
	mAnim.loop = false;

	mAttackInfo.damage = 12.0f;
	mAttackInfo.duration = mAttackTime;
	mAttackInfo.colRect.width = 80.0f;
	mAttackInfo.colRect.height = 70.0f;
	computeAttackRectPos(mAttackInfo.colRect);
	mAttackInfo.knockBack = 1000.0f;
	mAttackInfo.tag = DamageTag::MeleeHeavy;   // ★ 追加：重攻撃
	
	// TODO: プログラム体験③ ターゲットレイヤー変更
	mAttackInfo.targetMask = Actor::Type::Eenemy | Actor::Type::EstageObject;
	//mAttackInfo.targetMask = Actor::Type::Eenemy;
}

void ChargeAttack::update()
{
	computeAttackRectPos(mAttackInfo.colRect);
	// 当たり判定表示
	//DrawRectangleRec(mAttackInfo.colRect, LIGHTGRAY);

	mAttackTimer += GetFrameTime();
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		mPlayer->changeState(Type::Idle);
	}
}

void ChargeAttack::enter()
{
	PlayerState::enter();
	computeAttackRectPos(mAttackInfo.colRect);
	mPlayer->getAttackComp()->startAttack(&mAttackInfo);

	SoundSystem::instance().playSE("ChargeAtkSE");
}

Dying::Dying(PlayerActor* player)
	: PlayerState(player, Type::Dying)
{
	mAnim.frames = mPlayer->getSequence()->getAnimationFrames("player", "death", "png", 10);
	mAnim.loop = false;
	mAnim.fps = 15.0f;
}

void Dying::update()
{
	if (!mPlayer->getAnimSpriteComp()->isAnimating()) {
		mPlayer->onDead();
	}
}

