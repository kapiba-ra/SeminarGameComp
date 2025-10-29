#include "StageObject.h"
#include "GamePlay.h"
#include "SoundSystem.h"

#include "SpriteComponent.h"
#include "HpComponent.h"

#include "ExplosionActor.h"

StageObject::StageObject(Sequence* sequence, Type type)
	: Actor(sequence, Actor::Type::EstageObject)
	, mSpriteComp(nullptr)
	, mType(type)
{
	static_cast<GamePlay*>(mSequence)->addStageObj(this);
}

StageObject::~StageObject()
{
	static_cast<GamePlay*>(mSequence)->removeStageObj(this);
}

HardObj::HardObj(Sequence* sequence, int tileNum, Rectangle r)
	: StageObject(sequence, Type::Ehard)
	, mTileNum(tileNum)
{
	mRectangle = r;
	mPosition.x = r.x + r.width / 2.0;
	mPosition.y = r.y + r.height / 2.0;

	// タイルを元に,タイルサイズ分で結合したテクスチャを作成する
	//Texture2D* tex = mSequence->getTexture("Assets/FloorTile.png");
	//Image tileImage = LoadImageFromTexture(*tex);
	//Image combined = GenImageColor(tileImage.width * mTileNum, tileImage.height, BLANK);
	//for (int x = 0; x < mTileNum; x++) {
	//	Rectangle src = { 0, 0, (float)tileImage.width, (float)tileImage.height };
	//	Rectangle dest = { (float)(x * tileImage.width), 0.0f,
	//					  (float)tileImage.width, (float)tileImage.height };
	//	ImageDraw(&combined, tileImage, src, dest, WHITE);
	//}
	//// 出来たテクスチャをスプライトとして設定
	//mTexture = LoadTextureFromImage(combined);
	//mSpriteComp = new SpriteComponent(this);
	//mSpriteComp->setTexture(mTexture);

	//// イメージは破棄
	//UnloadImage(tileImage);
	//UnloadImage(combined);
}

HardObj::~HardObj()
{
	UnloadTexture(mTexture);
}

BreakableObj::BreakableObj(Sequence* sequence, Rectangle r)
	: StageObject(sequence, Type::Ebreakable)
{
	mRectangle = r;
	mPosition.x = r.x + r.width / 2.0;
	mPosition.y = r.y + r.height / 2.0;
	
	Texture2D* tex = mSequence->getTexture("Assets/taru.png");
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->setTexture(*tex);
	
	mHpComp = new HpComponent(this, 20.0f, 0.0f);
}

void BreakableObj::update()
{
	Actor::update();

	if (mHpComp->IsKilled()) {

		// --------------------------------------		
		SoundSystem::instance().playSE("ExplosionSE");
		Actor* actor = new ExplosionActor(mSequence);
		actor->setPosition(mPosition);
		actor->computeRectangle();
		// --------------------------------------

		setState(Edead);
	}
}
