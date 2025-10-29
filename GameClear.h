#pragma once
#include "Sequence.h"
/// <summary>
/// GameClearシーケンス
/// </summary>
class GameClear :
    public Sequence
{
public:
    GameClear();
    ~GameClear() override;

    void input() override;
    void update() override;
    void draw() override;
    Sequence* nextSequence() override;

private:
    Sequence* mNext = nullptr;
};

