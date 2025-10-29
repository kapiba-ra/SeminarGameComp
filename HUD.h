#pragma once
#include "UIScreen.h"
#include <raylib.h>
#include "PlayerState.h"

/// <summary>
/// HUDの表示を担当するクラス
/// </summary>
class HUD :
    public UIScreen
{
public:
    HUD(class Sequence* sequence);
    
    void update() override;
    void draw() override;

private:
    Rectangle mHpbar;       // hpバーの背景
    Rectangle mCurrentHp;   // 現在のhpを表示する
    PlayerState::Type mPlayerStateType;
};

