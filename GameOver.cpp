#include "GameOver.h"
#include "Title.h"

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

void GameOver::input()
{
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new Title();
	}
}

void GameOver::update()
{
}

void GameOver::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawText("GameOver", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> Title", 100, 200, 20, GRAY);

	EndDrawing();
}

Sequence* GameOver::nextSequence()
{
	return mNext;
}
