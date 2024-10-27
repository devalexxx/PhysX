//
// Created by Alex on 26/10/2024.
//

#include <PhysX/Constraint.h>
#include <PhysX/System.h>

Rope::Rope(float k, float length, System& left, System& right) :
	mK(k),
	mLength(length),
	mLeftForce(0.f),
	mRightForce(0.f),
	mLeft(left),
	mRight(right)
{
	left. AddForce(LForce(*this));
	right.AddForce(RForce(*this));
}

void Rope::Tick(float /* dt */)
{
	float d = glm::distance(mLeft.GetPosition(), mRight.GetPosition());
	float r = mK * (d - mLength);
	mLeftForce  = r * glm::normalize(mRight.GetPosition() - mLeft.GetPosition());
	mRightForce = r * glm::normalize(mLeft.GetPosition() - mRight.GetPosition());
}

Rope::LForce::LForce(const Rope& rope) :
	mRope(rope)
{}

void Rope::LForce::Tick(System* /* s */)
{
	mForce = mRope.mLeftForce;
}

Rope::RForce::RForce(const Rope& rope) :
	mRope(rope)
{}

void Rope::RForce::Tick(System* /* s */)
{
	mForce = mRope.mRightForce;
}
