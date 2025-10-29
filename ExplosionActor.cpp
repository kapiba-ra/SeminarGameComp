#include "ExplosionActor.h"
#include "Animation.h"
#include "AnimSpriteComponent.h"
#include "GamePlay.h"

#include "PlayerActor.h"
#include "EnemyActor.h"
#include "StageObject.h"
#include "HpComponent.h"
#include "Boss.h"

Animation ExplosionActor::mAnim = Animation{ {}, 24.0f, true };

ExplosionActor::ExplosionActor(Sequence* sequence)
	: Actor(sequence, Type::Eexplosion)
	, mTimer(0.0f)
	, mActiveTime(0.5f)
	, mDamageAmt(30.0f)
	, mActive(true)
	, mAnimsc(nullptr)
{
	mGamePlay = static_cast<GamePlay*>(sequence);
	//mAnimsc = new AnimSpriteComponent(this);
	//mAnim.frames = { mSequence->getTexture("Assets/testBomb.png") };
	//mAnim.loop = true;
	//mAnimsc->play(&mAnim);
}

void ExplosionActor::update()
{
	Actor::update();

	mTimer += GetFrameTime();
	if (mTimer > mActiveTime) {
		setState(Edead);
		return;
	}

	if (!mActive) return;
	Rectangle targetRec;
	for (auto enemy : mGamePlay->getEnemies()) {
		if (!enemy->getHpComp()->isInvincible()) {
			targetRec = enemy->getRectangle();
			if (CheckCollisionRecs(targetRec, mRectangle)) {
				if (auto boss = dynamic_cast<Boss*>(enemy)) {
					// ★ 爆発はガードに通りやすい
					boss->ApplyDamage(mDamageAmt, DamageTag::Explosion);
				} else {
					if (enemy->getHpComp()->TakeDamage(mDamageAmt)) {
						enemy->setState(Actor::Edead);
					}
				}
				mActive = false;
			}

		}
	}
}

void ExplosionActor::computeRectangle()
{
	mRectangle.x = mPosition.x - 40.0f / 2.0f;
	mRectangle.y = mPosition.y - 40.0f / 2.0f;
	mRectangle.width = 40.0f;
	mRectangle.height = 40.0f;
}
