#pragma once
#include "UIScreen.h"
#include <raylib.h>

class GamePlay;
class Boss;
class HpComponent;
class GuardComponent;

// ボス部屋専用のHUD (ボスHP/ガード表示)
class BossHUD : public UIScreen {
public:
    explicit BossHUD(GamePlay* gp);

    void update() override;
    void draw() override;

    // ボスを設定（入室時に指定）
    void setBoss(Boss* boss) { mBoss = boss; }

private:
    GamePlay* mGP{};
    Boss* mBoss{};

    // バーのレイアウト
    Rectangle mHpBarFrame{};
    Rectangle mGuardBarFrame{};
    float mBarWidth{ 640.0f };
    float mBarHeight{ 16.0f };
    float mBarGap{ 10.0f }; // HPとGuardの間隔

    // 内側（現在値）描画用
    Rectangle mHpBarNow{};
    Rectangle mGuardBarNow{};

    // テキスト
    const char* mHpLabel{ "BOSS HP" };
    const char* mGuardLabel{ "GUARD" };
};
