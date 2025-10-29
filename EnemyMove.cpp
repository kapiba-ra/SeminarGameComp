#include "EnemyMove.h"
#include "GamePlay.h"
#include "WeakEnemy.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(WeakEnemy* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(32.0f)
	, mAttackTime(1.0f)
	, mAttackTimer(100.0f)
	, mHitTime(0.5f)
	, mHitTimer(0.0f)
{
	mJumpSpeed = -600.0f;
	// �^�[�Q�b�g�w��(EnemyActor������w�肵�Ă�����)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	// ���R�����ւ̑J�ڔ�����ŏ��ɍs��
	if (mVelocityY > 0.0f &&
		mEnemy->getEnemyState() != WeakEnemy::E_jump &&
		mEnemy->getEnemyState() != WeakEnemy::E_hit)
	{
		mEnemy->changeState(WeakEnemy::E_jump);
	}

	// �d��(�����ړ�)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	mAttackTimer += GetFrameTime();
	// �X�e�[�g�ɂ���ď������ς��
	switch (mEnemy->getEnemyState())
	{
	case WeakEnemy::E_walk:
	{
		// ���g�̈ʒu�ɑ΂���v���C���[�̈ʒu�̕������擾(���g���Ȃ�-1�A���Ȃ�1�ɐi��)
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//�����ړ��̑��x��ݒ�
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		
		// �����˒�����
		if (withinRange()) {
			// �U���C���^�[�o�����I���Ă�����
			if (mAttackTime < mAttackTimer) {
				// attack��Ԃ�
				mEnemy->changeState(WeakEnemy::E_attack);
				mVelocityX = 0.0f;	// attack���ɑ��x��0�ɂ��Ă݂�
				mAttackTimer = 0.0f;
			}
		}
		break;
	}
	case WeakEnemy::E_jump:
	{
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//�����ړ��̑��x��ݒ�
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		break;
	}
	case WeakEnemy::E_attack:
	{
		// �˒��O�Ȃ�T��
		if (!withinRange()) {
			mEnemy->changeState(WeakEnemy::E_walk);
		}
		// �˒����ň�莞�Ԃ�������
		else if (mAttackTime < mAttackTimer) {
			// ���g�̈ʒu�ɑ΂���v���C���[�̈ʒu�̕�����ݒ�
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
			mEnemy->changeState(WeakEnemy::E_attack);
			mAttackTimer = 0.0f;
		}
		break;
	}
	case WeakEnemy::E_hit:
	{
		mHitTimer += GetFrameTime();
		if (mHitTime < mHitTimer) {
			mHitTimer = 0.0f;
			mEnemy->changeState(WeakEnemy::E_walk);
		}
	}
	}

	//���x���ʒu�ɔ��f
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();
	mOwner->setPosition(ownerPos);
	// Enemy�̈ړ������S�ɏI��������̈ʒu����ɒ����`�悷�邽�߂�Rectangle�ʒu����
	mOwner->computeRectangle();
}

void EnemyMove::fixFloorCol()
{
	// Y�����̑��x��������
	mVelocityY = 0.0f;
	// �󒆂���̉����̏ꍇ��State��ύX����
	if (mEnemy->getEnemyState() == WeakEnemy::E_jump) {
		mEnemy->changeState(WeakEnemy::E_walk);
	}
}

bool EnemyMove::withinRange()
{
	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	if (abs(ownerPos.x - targetPos.x) < mAttackRange &&
		abs(ownerPos.y - targetPos.y) < mAttackRange) {
		return true;
	}
	else {
		return false;
	}
}
