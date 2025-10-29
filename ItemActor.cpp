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
	
	// �v���C���[�Ƃ̏Փ˔�����s��
	Rectangle playerRec = static_cast<GamePlay*>(mSequence)->getPlayer()->getRectangle();
	if (CheckCollisionRecs(playerRec, mRectangle)) {
		// �A�C�e���̌��ʂ��v���C���[�ɔ��f
		onAcquired();
		// �A�C�e���������܂�
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
	// �G��ݒ�
	Texture2D* tex = mSequence->getTexture("Assets/SpeedUpItem.png");
	mSpriteComp->setTexture(*tex);
	// �����蔻��p�̋�`��ݒ�
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void SpeedUpItem::onAcquired()
{
	// �ʓ|�ł���,mSequence��GamePlay�ɃL���X�g����K�v������܂�
	// Player�̈ړ��𐧌䂷��Component��,�ړ����x�{���ƌp�����Ԃ�ݒ肵�܂�
	static_cast<GamePlay*>(mSequence)->getPlayer()
		->getPlayerMove()->setMultiplier(mBuffMultiplier, mDuration);

	// Item�p��SE��炷
	SoundSystem::instance().playSE("ItemSE");
}

// TODO: �v���O�����ۑ�Ex �񕜃A�C�e���̎���
// �񕜃A�C�e���̃e�N�X�`����Assets�t�H���_�ɂ���HealingItem.png�ł��B
// �v���C���[��Hp���񕜂���ɂ�,�v���C���[��HpComonent���擾���A��������Recover()�֐����Ăяo���Ă��������B
// �v���C���[�̗̑͂�100.0f�ł�

/* �ȉ��q���g */
// HpComponent.h�̃C���N���[�h���Y��Ȃ��悤�ɂ��܂��傤
// Recover�֐��ɂ͉񕜗ʂ������Ƃ��ēn���܂��傤

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
