#pragma once

/// <summary>
/// UI‚ÌŠî’ê
/// </summary>
class UIScreen
{
public:
	UIScreen(class Sequence* sequence);
	virtual ~UIScreen() {};

	virtual void update() {};
	virtual void draw() {};

	// ó‘ÔŠÇ—
	enum UIState
	{
		EActive,
		EClosing
	};
	void Close();
	UIState GetState() const { return mState; }

protected:
	class Sequence* mSequence;
	UIState mState;
};
