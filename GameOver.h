#pragma once
#include "Sequence.h"
class GameOver :
    public Sequence
{
public:
    GameOver();
    ~GameOver() override;

    void input() override;
    void update() override;
    void draw() override;
    Sequence* nextSequence() override;

private:
    Sequence* mNext = nullptr;
};

