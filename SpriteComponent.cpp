#include "SpriteComponent.h"
#include "Actor.h"
#include "GamePlay.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture()
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	static_cast<GamePlay*>(mOwner->getSequence())->addSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	static_cast<GamePlay*>(mOwner->getSequence())->removeSprite(this);
}

void SpriteComponent::draw()
{
	if (IsTextureValid(mTexture))
	{
		Vector2 pos = mOwner->getPosition();
		float scale = mOwner->getScale();
		float forward = (float)mOwner->getForward();
		// 描画
		Rectangle src = { 0, 0, forward * (float)mTexWidth, (float)mTexHeight };
		Rectangle dst = { pos.x, pos.y, (float)mTexWidth * scale, (float)mTexHeight * scale };
		Vector2 origin = { mTexWidth * scale / 2.0f, mTexHeight * scale / 2.0f };
		DrawTexturePro(mTexture, src, dst, origin, 0.0f, WHITE);
	}
}

void SpriteComponent::setTexture(Texture2D texture)
{
	// Draw関数が使えるように (mTextureがnullptrではなくなり,trueを返す)
	mTexture = texture;
	// テクスチャの高さと幅を求める
	mTexWidth = mTexture.width;
	mTexHeight = mTexture.height;
}
