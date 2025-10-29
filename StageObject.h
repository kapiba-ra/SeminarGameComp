#pragma once
#include "Actor.h"

/// <summary>
/// �X�e�[�W���\������I�u�W�F�N�g�̊��N���X
/// </summary>
class StageObject :
    public Actor
{
public:
    enum Type
    {
        Ehard,
        Ebreakable
    };
    StageObject(class Sequence* sequence, Type type);
    virtual ~StageObject();

    class HpComponent* getHpComp() { return mHpComp; }
    Type getType() const { return mType; }

protected:
    Type mType;
    class SpriteComponent* mSpriteComp;
    class HpComponent* mHpComp;
};

/// <summary>
/// ���Ƃ���
/// �������ň��Actor�Ƃ��Ă܂Ƃ߂Ă���
/// </summary>
class HardObj :
    public StageObject
{
public:
    HardObj(class Sequence* sequence, int tileNum, Rectangle r);
    ~HardObj() override;

private:
    const int mTileNum;    // (������)�����\������^�C���̐�
    Texture2D mTexture;    // �V�[�P���X�Ɏ������Ă�������,����ēo�^�����Ԃ͏Ȃ�
};

/// <summary>
/// �j��\�I�u�W�F�N�g(����)
/// </summary>
class BreakableObj :
    public StageObject
{
public:
    BreakableObj(class Sequence* sequence, Rectangle r);
    void update() override;
};