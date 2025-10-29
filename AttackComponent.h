#pragma once
#include "Component.h"
#include "AttackInfo.h"

struct KnockbackInfo
{
    class Actor* target;    // �m�b�N�o�b�N�ňړ�����Ώ�
    Vector2 velocity;       // �m�b�N�o�b�N�����Ƒ��x
    float timer;            // �m�b�N�o�b�N����
};

/// <summary>
/// �U�����s��(HP�����炷,�m�b�N�o�b�N�t�^,���ʉ��炷)�@�\��t�^����component
/// �O������AttackInfo���󂯎��A����Ɋ�Â�������������
/// </summary>
class AttackComponent :
    public Component
{
public:
    AttackComponent(class Actor* owner);

    void update() override;
    // �U������ۂɌĂяo��,�U�����ɂ��Ăׂ�
    void startAttack(AttackInfo* info);

private:
    void processAttackEnemy();
    void processAttackPlayer();
    void processAttackStageObj();
    float kbNormCalculator(int kbType, float damage); // 2025/10/21�ǉ� �m�b�N�o�b�N�̏����x�v�Z
    
    float mTimer;
    // ���݂̍U�����
    AttackInfo* mCurInfo;
    bool mActive;

    std::vector<KnockbackInfo> mKnockbackTargets;
    std::vector<Actor*> mHitActors;
};

