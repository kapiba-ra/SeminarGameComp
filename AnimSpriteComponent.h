#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "Animation.h"

/// <summary>
/// アニメーション付きスプライトの描画
/// </summary>
class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    // 一定のフレームごとにアニメーションを更新 componentからoverride
    void update() override;

    // アニメーションを再生する
    void play(const Animation* anim);

    bool isAnimating() const { return mIsAnimating; }

private:
    // アニメーションのマップ
    std::unordered_map<std::string, Animation> mAnimations;
    const Animation* mCurrentAnim;  // 現在のアニメーション
    // 現在表示しているフレーム
    float mCurrFrame;
    bool mIsAnimating;
};
