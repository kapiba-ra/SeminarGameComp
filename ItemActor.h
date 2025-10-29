#pragma once
#include "Actor.h"

/// <summary>
/// �A�C�e���̊��N���X
/// �v���C���[�ɓ�����Ɖe�����y�ڂ�
/// �񕜂ƃX�s�[�h�A�b�v�̓�̃A�C�e��������
/// </summary>
class ItemActor :
    public Actor
{
public:
    ItemActor(class Sequence* sequence);
    virtual ~ItemActor() {};

    // �v���C���[�Ƃ̏Փ˔�����s���Ă��܂�
    void update() override;
    // �����蔻��̍X�V
    void computeRectangle() override;
    // �v���C���[���A�C�e���G�ꂽ���ɌĂяo�����֐�
    // �������z�֐��ɂ����̂Ŕh���N���X�ŕK���������Ȃ��Ă͂Ȃ�Ȃ�)
    virtual void onAcquired() = 0;

protected:
    class SpriteComponent* mSpriteComp;
};

/// <summary>
/// �ړ����x�㏸�A�C�e��
/// </summary>
class SpeedUpItem
    : public ItemActor
{
public:
    SpeedUpItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mDuration;        // ���ʎ���
    float mBuffMultiplier;  // �{���Ńo�t���|����
};

// TODO: �v���O�����ۑ�Ex �񕜃A�C�e���̎���
// �ڂ��������́u�v���O���~���O�̌�Ex����.docx�v�ɏ悹�Ă��܂�
// 1.ItemActor.h, ItemActor.cpp�ɉ񕜃A�C�e������������
// 2.WeekEnemy.cpp�G���G�����S�����Ƃ��Ɋm���ŉ񕜃A�C�e���𗎂Ƃ��悤�ɂ���
// �Ƃ�������ł�
// SpeedUpItem���Q�l�ɍ���Ă݂ĉ�����

/// <summary>
/// �񕜃A�C�e��
/// </summary>
class HealingItem
    : public ItemActor
{
public:
    HealingItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mHealAmount;
};