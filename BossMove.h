#pragma once
//===============================================
// BossMove.h
//  EnemyMove を踏襲したボス用 MoveComponent。
//  - プレイヤー方向へ歩行（一定の間合いを維持）
//  - 重力/床衝突
//  - 低い段差の“乗り上げ”（step up）
//  - 壁衝突で押し戻し
//===============================================
#include "MoveComponent.h"
#include <vector>
#include <raylib.h>

class Boss;
class GamePlay;
class Stage;
class PlayerActor;

class BossMove : public MoveComponent {
public:
    explicit BossMove(Boss* owner);

    void update() override;     // 物理＋追尾AI
    void fixFloorCol();         // 床に接地したときに呼ぶ

    // 調整用
    void setKeepRange(float r) { mKeepRange = r; }
    void setWalkSpeed(float s) { mMoveSpeed = s; }
    void setGravity(float g)   { mGravity   = g; }

private:
    // 衝突解決のヘルパ
    void resolveVertical(const std::vector<Rectangle>& stageRecs);
    void resolveHorizontal(const std::vector<Rectangle>& stageRecs);
    bool tryStepUp(const Rectangle& stageRec, const std::vector<Rectangle>& stageRecs, float tileSize);

private:
    Boss*   mBoss;
    GamePlay*    mGP;
    PlayerActor* mPlayer;

    // AI調整
    float mKeepRange;   // これ以上遠いと寄る
    int   mTileSize;
};
