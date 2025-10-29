#include "Title.h"
#include "GamePlay.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::input()
{
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new GamePlay(); // 次のシーケンスをGamePlayにセット
	}
}

void Title::update()
{
}

void Title::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawText("TITLE", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> GamePlay", 100, 200, 20, GRAY);

	DrawText("Attack : Mouse left click", 150, 260, 20, PINK);
	DrawText("Jump : SpaceKey", 150, 300, 20, PINK);
	DrawText("Move : AD", 150, 340, 20, PINK);
	DrawText("Dodge : Press the move button twice", 150, 380, 20, PINK);
	DrawText("Charge : Mouse left Long-press", 150, 420, 20, PINK);
	DrawText("Guard : Mouse right press", 150, 460, 20, PINK);
	

	EndDrawing();
}

Sequence* Title::nextSequence()
{
	return mNext;
}
