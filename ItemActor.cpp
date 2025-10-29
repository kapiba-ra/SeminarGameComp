#include "ItemActor.h"
#include "GamePlay.h"
#include "SoundSystem.h"

#include "SpriteComponent.h"
#include "PlayerActor.h"
#include "HpComponent.h"
#include "PlayerMove.h"

ItemActor::ItemActor(Sequence* sequence)
	: Actor(sequence, Type::Eitem)
{
	mSpriteComp = new SpriteComponent(this);
}

void ItemActor::update()
{
	Actor::update();
	
	// プレイヤーとの衝突判定を行う
	Rectangle playerRec = static_cast<GamePlay*>(mSequence)->getPlayer()->getRectangle();
	if (CheckCollisionRecs(playerRec, mRectangle)) {
		// アイテムの効果をプレイヤーに反映
		onAcquired();
		// アイテムを消します
		setState(Edead);
	}
}

void ItemActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
	mRectangle.y = mPosition.y - mRectangle.height / 2.0f;
}



SpeedUpItem::SpeedUpItem(Sequence* sequence)
	: ItemActor(sequence)
	, mDuration(15.0f)
	, mBuffMultiplier(1.5f)
{
	// 絵を設定
	Texture2D* tex = mSequence->getTexture("Assets/SpeedUpItem.png");
	mSpriteComp->setTexture(*tex);
	// 当たり判定用の矩形を設定
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void SpeedUpItem::onAcquired()
{
	// 面倒ですが,mSequenceをGamePlayにキャストする必要があります
	// Playerの移動を制御するComponentの,移動速度倍率と継続時間を設定します
	static_cast<GamePlay*>(mSequence)->getPlayer()
		->getPlayerMove()->setMultiplier(mBuffMultiplier, mDuration);

	// Item用のSEを鳴らす
	SoundSystem::instance().playSE("ItemSE");
}

// TODO: プログラム課題Ex 回復アイテムの実装
// 回復アイテムのテクスチャはAssetsフォルダにあるHealingItem.pngです。
// プレイヤーのHpを回復するには,プレイヤーのHpComonentを取得し、そこからRecover()関数を呼び出してください。
// プレイヤーの体力は100.0fです

/* 以下ヒント */
// HpComponent.hのインクルードも忘れないようにしましょう
// Recover関数には回復量を引数として渡しましょう

HealingItem::HealingItem(Sequence* sequence)
	: ItemActor(sequence)
	, mHealAmount(20.0f)
{
	Texture2D* tex = mSequence->getTexture("Assets/HealingItem.png");
	mSpriteComp->setTexture(*tex);
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void HealingItem::onAcquired()
{
	static_cast<GamePlay*>(mSequence)->getPlayer()->
		getHpComp()->Recover(mHealAmount);

	SoundSystem::instance().playSE("ItemSE");
}
