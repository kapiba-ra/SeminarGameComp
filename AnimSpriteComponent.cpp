#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mIsAnimating(false)
	, mCurrentAnim(nullptr)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	if (mCurrentAnim && mIsAnimating)
	{
		// フレームを進めていく
		mCurrFrame += mCurrentAnim->fps / GetFPS();
		// フレーム数を超えたら
		if (mCurrFrame >= (int)mCurrentAnim->frames.size())
		{
			// ループアニメーションの場合
			if (mCurrentAnim->loop) {
				// 最初のフレームに補正付きで戻る
				mCurrFrame = fmod(mCurrFrame, (float)mCurrentAnim->frames.size());
			}
			// ループしない場合
			else {
				// 最終フレームに設定
				mCurrFrame = (float)(mCurrentAnim->frames.size() - 1);
				// アニメーション終了
				mIsAnimating = false;
			}
		}
		// 表示テクスチャ更新
		setTexture(*mCurrentAnim->frames[(int)mCurrFrame]);
	}
}

void AnimSpriteComponent::play(const Animation* anim)
{
	if (!anim) return;
	if (!anim->frames.empty()) {
		mCurrentAnim = anim;
		mCurrFrame = 0.0f;
		mIsAnimating = true;
		setTexture(*anim->frames[0]);
	}
}
