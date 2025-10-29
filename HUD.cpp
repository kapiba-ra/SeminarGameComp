#include "HUD.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "HpComponent.h"

HUD::HUD(Sequence* sequence)
	: UIScreen(sequence)
{
	// hp�o�[
	mHpbar = { 10.0f, 50.0f, 200.0f, 10.0f }; // x,y,width,height
	mCurrentHp = mHpbar;
}

void HUD::update()
{
	PlayerActor* player = static_cast<GamePlay*>(mSequence)->getPlayer();
	if (!player) return;
	// ���݂�player��hp�ɉ�����,�\�����镝��ς���
	// �ʒm->�X�V�ł͂Ȃ�,update�Ŗ���X�V����悤�ɂ��Ă���
	mCurrentHp.width = mHpbar.width * player->getHpComp()->GetHpRatio();
	mPlayerStateType = player->getPlayerState()->getType();
}

void HUD::draw()
{
	/* HPbar */
	// "hp" ����
	DrawText("hp", (int)mHpbar.x, (int)mHpbar.y - 10, 10, BLACK);
	// �w�i (�g)
	DrawRectangleRec(mHpbar, GRAY);
	// HP�ɉ������o�[,20�p�[�ȉ��ŐF��RED��
	DrawRectangleRec(mCurrentHp, 
		mCurrentHp.width > mHpbar.width / 5 ? GREEN : RED);

	/* Player�̃X�e�[�g */
	//switch (mPlayerStateType)
	//{
	//case PlayerState::Type::Idle: {
	//	DrawText("Player : Idle", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::Walk: {

	//	DrawText("Player : Walk", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::Jump: {
	//	DrawText("Player : Jump", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::Dodge: {
	//	DrawText("Player : Dodge", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::Guard: {
	//	DrawText("Player : Guard", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::Charge: {
	//	DrawText("Player : Charge", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::NormalAttack: {
	//	DrawText("Player : nAttack", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::DodgeAttack: {
	//	DrawText("Player : dAttack", 700, 50, 30, BLACK); break;
	//}
	//case PlayerState::Type::ChargeAttack: {
	//	DrawText("Player : cAttack", 700, 50, 30, BLACK); break;
	//}
	//}
}
