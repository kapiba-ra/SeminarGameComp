#include "BossHUD.h"
#include "GamePlay.h"
#include "Boss.h"
#include "HpComponent.h"
#include "GuardComponent.h"
#include <algorithm>
#include <raylib.h>

BossHUD::BossHUD(GamePlay* gp)
    : UIScreen(gp)
    , mGP(gp)
{
    // 画面上部中央に 2 本のバーを並べる
    const int sw = GetScreenWidth();
    const float x = (sw * 0.5f) - (mBarWidth * 0.5f);
    const float y = 20.0f;

    mHpBarFrame    = { x,             y,                       mBarWidth, mBarHeight };
    mGuardBarFrame = { x,             y + mBarHeight + mBarGap, mBarWidth, mBarHeight };

    mHpBarNow    = mHpBarFrame;
    mGuardBarNow = mGuardBarFrame;
}

void BossHUD::update() {
    if (!mBoss) { Close(); return; }

    // HP/Guard の比率を取り、バー幅を更新
    float hpRatio    = 0.0f;
    float guardRatio = 0.0f;
    auto* hp = mBoss->getHpComp();
    if (hp) {
        hpRatio = std::max(0.0f, std::min(hp->GetHpRatio(), 1.0f));
    }
    auto* guard = mBoss->getGuardComp();
    if (guard) {
        guardRatio = std::max(0.0f, std::min(guard->GetRatio(), 1.0f));
    }

    mHpBarNow   = mHpBarFrame;
    mHpBarNow.width = mHpBarFrame.width * hpRatio;

    mGuardBarNow = mGuardBarFrame;
    mGuardBarNow.width = mGuardBarFrame.width * guardRatio;

    // ボスが倒れたら自分を閉じる
    if (hpRatio <= 0.0f) {
        Close();
    }
}

void BossHUD::draw() {
    if (!mBoss) return;

    // フレーム
    DrawRectangleLinesEx(mHpBarFrame, 2.0f, BLACK);
    DrawRectangleLinesEx(mGuardBarFrame, 2.0f, BLACK);

    // 中身（色はお好みで）
    DrawRectangleRec(mHpBarNow, RED);        // HPは赤
    DrawRectangleRec(mGuardBarNow, BLUE);    // Guardは青

    // ラベル
    DrawText(mHpLabel,    (int)mHpBarFrame.x,    (int)(mHpBarFrame.y - 18),    18, BLACK);
    DrawText(mGuardLabel, (int)mGuardBarFrame.x, (int)(mGuardBarFrame.y - 18), 18, BLACK);
}
