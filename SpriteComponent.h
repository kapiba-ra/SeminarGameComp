#pragma once
#include "Component.h"
#include <raylib.h>

/// <summary>
/// �X�v���C�g�`��p��Component
/// 1���̃e�N�X�`����`�悷��Ȃ炱�����ł���
/// �����e�N�X�`���̕`��A�����̓A�j���[�V�����t���̃X�v���C�g��AnimSpriteComponent
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
    // �`�悷��e�N�X�`��
    Texture2D mTexture;

    int mDrawOrder;
    int mTexHeight;
    int mTexWidth;
};

