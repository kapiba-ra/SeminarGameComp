#include "UIScreen.h"
#include "Sequence.h"
#include <raylib.h>

UIScreen::UIScreen(Sequence* sequence)
	: mSequence(sequence)
	, mState(EActive)
{
	// UIスタックにUIを追加
	mSequence->pushUI(this);
}

void UIScreen::Close()
{
	mState = EClosing;
}
