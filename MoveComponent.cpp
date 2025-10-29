#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner)
	: Component(owner)
	, mGravity(60.0f)
	, mMoveSpeed(180.0f)
	, mJumpSpeed(-1200.0f)
	, mVelocityX(0.0f)
	, mVelocityY(0.0f)
{
}

