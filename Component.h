#pragma once

/// <summary>
/// ComponentŠî’êƒNƒ‰ƒX
/// Œp³‚µ‚Ä‰½‚©ì‚é‚Æ‚«‚Í,’P‚ÉActor‚É‚½‚¹‚é‚¾‚¯‚Å‚¢‚¢(‚Í‚¸)
/// </summary>
class Component
{
public:
	Component(class Actor* owner);
	virtual ~Component();

	virtual void update();
	virtual void input() {}

protected:
	class Actor* mOwner;
};

