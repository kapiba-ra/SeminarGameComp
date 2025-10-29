#include "Actor.h"
#include "Sequence.h"
#include "Component.h"

Actor::Actor(Sequence* sequence, Type type)
	: mSequence(sequence)
	, mType(type)
	, mState(Ealive)
	, mPosition(Vector2())
	, mScale(1.0f)
	, mRectangle()
	, mForward(1)
{
	mSequence->addActor(this);
}

Actor::~Actor()
{
	mSequence->removeActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::input()
{
	for (auto comp : mComponents) {
		comp->input();
	}
}

void Actor::update()
{
	for (auto comp : mComponents) {
		comp->update();
	}
}

void Actor::addComponent(Component* component)
{
	mComponents.emplace_back(component);
}

void Actor::removeComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
