#pragma once
#include "Component.h"
#include <raylib.h>

/// <summary>
/// スプライト描画用のComponent
/// 1枚のテクスチャを描画するならこっちでいい
/// 複数テクスチャの描画、或いはアニメーション付きのスプライトはAnimSpriteComponent
/// </summary>
class SpriteComponent :
    public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();
    virtual void draw();
    virtual void setTexture(Texture2D texture);
    int getDrawOrder() const { return mDrawOrder; }
    int getTexHeight() const { return mTexHeight; }
    int getTexWidth() const { return mTexWidth; }

protected:
    // 描画するテクスチャ
    Texture2D mTexture;

    int mDrawOrder;
    int mTexHeight;
    int mTexWidth;
};

