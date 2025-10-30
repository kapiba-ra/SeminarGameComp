#include "WeakEnemy.h"
#include "GamePlay.h"
#include "Stage.h"

#include "EnemyMove.h"
#include "AnimSpriteComponent.h"
#include "HpComponent.h"
#include "AttackComponent.h"
// weapon
#include "WeaponActor.h"
#include "PlayerActor.h"
#include "ArrowActor.h"
#include "ItemActor.h"
#include "StageObject.h"

WeakEnemy::WeakEnemy(Sequence* sequence)
	: EnemyActor(sequence, Type::Weak)
    , mEnemyState(E_walk)
{
    mScale = 2.0f;
    mEnemyMove = new EnemyMove(this);
    mEnemyMove->setMoveSpped(60.0f);
	mAnimsc = new AnimSpriteComponent(this);
    mHpComp->SetInvincibleDuration(0.0f);

    mRectangle.width = 21.0f * mScale;
    mRectangle.height = 35.0f * mScale;
}

void WeakEnemy::changeState(EnemyState nextState)
{
    onExitState(nextState);
    onEnterState(nextState);
}

void WeakEnemy::computeRectangle()
{
    mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
    mRectangle.y = mPosition.y - mRectangle.height / 2.0f;
}

void WeakEnemy::onEnterState(EnemyState nextState)
{
    // �X�e�[�g�ύX
    mEnemyState = nextState;
    // �X�e�[�g�ύX���̏������Ă�
    switch (mEnemyState)
    {
    case E_walk:
        mAnimsc->play(&getAnimation(E_walk));
        break;
    case E_jump:
        mAnimsc->play(&getAnimation(E_jump));
        break;
    case E_attack:
        mAnimsc->play(&getAnimation(E_attack));
        attack();
        break;
    case E_hit:
        mAnimsc->play(&getAnimation(E_hit));
        break;
    }
}

void WeakEnemy::onExitState(EnemyState nextState)
{
    switch (mEnemyState)
    {
    case E_walk:
        break;
    case E_jump:
        break;
    case E_attack:
        break;
    case E_hit:
        break;
    }
}

void WeakEnemy::jump()
{
    if (mEnemyState == E_walk) {
        changeState(E_jump);
        mEnemyMove->setVelocityY(mEnemyMove->getJumpSpeed());
    }
}

void WeakEnemy::computeAttackRectPos(Rectangle& rec)
{
    if (mForward > 0) rec.x = mRectangle.x + mRectangle.width;
    else rec.x = mRectangle.x - rec.width;
    rec.y = mRectangle.y;
}

void WeakEnemy::dropItem()
{
    // TODO:���񂾂�A�C�e���������_���ŗ��Ƃ�
    Actor* item = nullptr;
    int randomValue = GetRandomValue(1, 100);
    if (randomValue <= 15) {
        item = new HealingItem(mSequence);
    }
    else if (randomValue <= 30) {
        item = new SpeedUpItem(mSequence);
    }
    // �ʒu��ݒ�(�ύX�s�v)
    if (item) {
        item->setPosition(mPosition);
        item->computeRectangle();
    }
    //Actor* item = nullptr;
    //int randomValue = GetRandomValue(1, 100); // 1-100�܂ł̐����������_���œ���
    //if (randomValue < 15) {
    //    new SpeedUpItem(mSequence);
    //    // �G�̈ʒu�ɐݒ�
    //    item->setPosition(mPosition);
    //    item->computeRectangle();
    //}
}

void WeakEnemy::fixCollision()
{
    // �X�e�[�W�Ƃ̓����蔻��
    for (auto& stageObj : static_cast<GamePlay*>(mSequence)->getStageObjs()) {
        Rectangle stageRec = stageObj->getRectangle();
        if (CheckCollisionRecs(mRectangle, stageRec)) {
            Rectangle colRec = GetCollisionRec(mRectangle, stageRec);
            // �������̏Փ�
            if (colRec.width < colRec.height) {
                if (mRectangle.x < colRec.x) mPosition.x -= colRec.width;
                else mPosition.x += colRec.width;

                // �ǂɂԂ������Ƃ��̒i���`�F�b�N�i�W�����v������j
                if (mEnemyState != E_jump)
                {
                    const int tileSize = 40;
                    bool isStep = false;
                    if (mPosition.x < stageRec.x && mForward > 0 ||
                        mPosition.x > stageRec.x && mForward < 0) {
                        // �i����1.5�^�C���ȓ��Ȃ�W�����v
                        isStep = (stageRec.height <= tileSize * 1.5f);
                    }
                    // �i���̏�ɃX�y�[�X�����邩�m�F
                    Rectangle checkOneAbove = {
                        colRec.x,
                        stageRec.y - tileSize,
                        colRec.width,
                        1.0f
                    };
                    bool isSpaceAboveClear = true;
                    for (const auto& otherStageObj : static_cast<GamePlay*>(mSequence)->getStageObjs()) {
                        if (CheckCollisionRecs(checkOneAbove, otherStageObj->getRectangle())) {
                            isSpaceAboveClear = false;
                            break;
                        }
                    }
                    if (isStep && isSpaceAboveClear) {
                        jump();
                        break;
                    }
                }
            }
            // �c�����̏Փ�
            else if (colRec.width >= colRec.height) {
                // �ォ��Փ�
                if (mRectangle.y < colRec.y) {
                    mPosition.y -= colRec.height;
                    mEnemyMove->fixFloorCol();
                }
                // ������Փ�
                else {
                    mPosition.y += colRec.height;
                }
            }
            // �����蔻���`�̍X�V
            computeRectangle();
        }
    }
}

std::unordered_map<WeakEnemy::EnemyState, Animation> MeleeEnemy::mAnimations = {
    { WeakEnemy::E_walk,   Animation{{}, 24.0f, true} },
    { WeakEnemy::E_jump,   Animation{{}, 24.0f, false} },
    { WeakEnemy::E_attack, Animation{{}, 24.0f, false} },
};

MeleeEnemy::MeleeEnemy(Sequence* sequence)
	: WeakEnemy(sequence)
{
    // �A�j���[�V������ݒ�,�������new����x�ɌĂ�ł���
    // ���񃍁[�h����킯�ł͂Ȃ��̂łƂ肠�����Ë�

    // Walk
    mAnimations[E_walk].frames = mSequence->getAnimationFrames("enemy/melee", "dash", "png", 6);
    mAnimations[E_walk].loop = true;
    mAnimations[E_walk].fps = 10.0f;
    // attack
    mAnimations[E_attack].frames = mSequence->getAnimationFrames("enemy/melee", "attack", "png", 5);
    mAnimations[E_attack].loop = false;
    // jump
    mAnimations[E_jump].frames = mSequence->getAnimationFrames("enemy/melee", "jump", "png", 3);
    mAnimations[E_jump].loop = false;
    mAnimations[E_jump].fps = 20.0f;
    // hit
    mAnimations[E_hit].frames = mSequence->getAnimationFrames("enemy/melee", "hit", "png", 3);
    mAnimations[E_hit].loop = false;
    mAnimations[E_hit].fps = 10.0f;

    // �U���͋ߐ�
    mAttackComp = new AttackComponent(this);
    // �U�����̐ݒ�
    mAttackInfo.damage = 10.0f;
    mAttackInfo.duration = 0.3f;
    mAttackInfo.colRect.width = 40.0f;
    mAttackInfo.colRect.height = 60.0f;
    computeAttackRectPos(mAttackInfo.colRect);
    mAttackInfo.knockBack = 0.0f;
    mAttackInfo.targetMask = Actor::Type::Eplayer;

    mEnemyMove->setAttackRange(60.0f);
}

void MeleeEnemy::update()
{
    Actor::update();
    fixCollision();
    computeAttackRectPos(mAttackInfo.colRect);

    if (mHpComp->IsKilled()) {
        dropItem();
    }
    // �����蔻��\��
    if (mEnemyState == E_attack) {
        DrawRectangleRec(mAttackInfo.colRect, WHITE);

    }
}

void MeleeEnemy::attack()
{
    mAttackComp->startAttack(&mAttackInfo);
}

std::unordered_map<WeakEnemy::EnemyState, Animation> RangedEnemy::mAnimations = {
    { WeakEnemy::E_walk,   Animation{{}, 24.0f, true} },
    { WeakEnemy::E_jump,   Animation{{}, 24.0f, false} },
    { WeakEnemy::E_attack, Animation{{}, 24.0f, false} },
};

RangedEnemy::RangedEnemy(Sequence* sequence)
    : WeakEnemy(sequence)
{
    // �A�j���[�V������ݒ�
    std::vector<Texture2D*> frames;

    // Walk
    mAnimations[E_walk].frames = mSequence->getAnimationFrames("enemy/ranged", "walk", "png", 6);
    mAnimations[E_walk].loop = true;
    mAnimations[E_walk].fps = 10.0f;
    // attack
    mAnimations[E_attack].frames = mSequence->getAnimationFrames("enemy/ranged", "attack", "png", 8);
    mAnimations[E_attack].loop = false;
    // jump
    mAnimations[E_jump].frames = mSequence->getAnimationFrames("enemy/ranged", "jump", "png", 3);
    mAnimations[E_jump].loop = false;
    mAnimations[E_jump].fps = 20.0f;
    // hit
    mAnimations[E_hit].frames = mSequence->getAnimationFrames("enemy/ranged", "hit", "png", 3);
    mAnimations[E_hit].loop = false;
    mAnimations[E_hit].fps = 10.0f;

    mEnemyMove->setAttackRange(400.0f);
    mEnemyMove->setAttackTime(3.0f);
}

void RangedEnemy::update()
{
    Actor::update();
    fixCollision();
    if (mHpComp->IsKilled()) {
        dropItem();
    }
    // �����蔻��\��
    //DrawRectangleRec(mRectangle, WHITE);
}

void RangedEnemy::onEnterState(EnemyState nextState)
{
    WeakEnemy::onEnterState(nextState);

    if (nextState == E_attack) {
         ArrowActor* a = new ArrowActor(mSequence, Actor::Eenemy);
         a->setPosition(mPosition);
         a->computeRectangle();
         a->setForward(mForward);
         a->setVelocity(Vector2{ 700.0f * mForward, 0.0f });
    }
}
