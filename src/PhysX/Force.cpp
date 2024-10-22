//
// Created by Alex on 21/10/2024.
//

#include "PhysX/Force.h"
#include "PhysX/System.h"

Force::Force() : mForce(0.f) {}

const glm::vec3& Force::Get() const
{
	return mForce;
}

Draft::Draft(glm::vec3 coef) :
	mCoef(coef)
{}

void Draft::Tick(System* s)
{
	const auto& v = s->GetVelocity();
	if (float norm = glm::length(v); norm != 0)
		mForce = mCoef * norm * v;
	else
		mForce = glm::vec3(0, 0, 0);
}

Gravity::Gravity(float gravity) :
	mGravity(gravity)
{}

void Gravity::Tick(System* s)
{
	mForce = s->GetMass() * mGravity * glm::vec3(0, -1, 0);
}

Rope::Rope(float k, float length, glm::vec3 anchor) :
	mK(k), mLength(length), mAnchor(anchor)
{}

void Rope::Tick(System* s)
{
	float d = glm::distance(s->GetPosition(), mAnchor);
	float r = mK * (d - mLength);
	mForce = r * (mAnchor - s->GetPosition());
}
