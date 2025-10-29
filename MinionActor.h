#pragma once
//===============================================
// MinionActor.h
//  召喚された雑魚。自滅タイマで消え、ボスのHPを回復する。
//===============================================
#include "EnemyActor.h"

class Boss;
class GamePlay;

class MinionActor : public EnemyActor {
public:
    // life : 何秒後に自滅するか
    // heal : 自滅時にボスへ与える回復量
    MinionActor(GamePlay* seq, Boss* ownerBoss, float life, float heal);

    void update() override; // 自滅タイマと回復

private:
    Boss* mBoss   = nullptr; // 回復先のボス
    float      mLife   = 0.0f;    // 残り寿命（秒）
    float      mHealAmt= 0.0f;    // 回復量
};
