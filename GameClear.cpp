#include "GameClear.h"
#include "Title.h"

GameClear::GameClear()
{
}

GameClear::~GameClear()
{
}

void GameClear::input()
{
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new Title();
	}
}

void GameClear::update()
{
}

void GameClear::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawText("GameClear", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> Title", 100, 200, 20, GRAY);

	EndDrawing();
}

Sequence* GameClear::nextSequence()
{
	return mNext;
}
