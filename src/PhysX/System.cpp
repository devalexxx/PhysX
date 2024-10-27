//
// Created by Alex on 21/10/2024.
//

#include "PhysX/System.h"

System::System(float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration) :
	mMass(mass), mPosition(position), mVelocity(velocity), mAcceleration(acceleration)
{}

System::System(float mass) : System(mass, glm::vec3(0), glm::vec3(0), glm::vec3(0)) {}

float System::GetMass() const
{
	return mMass;
}

const glm::vec3& System::GetPosition() const
{
	return mPosition;
}

const glm::vec3& System::GetVelocity() const
{
	return mVelocity;
}

const glm::vec3& System::GetAcceleration() const
{
	return mAcceleration;
}

void EulerSystem::Tick(float dt)
{
	glm::vec3 tForce(0.f);
	for (auto& f: mForces)
	{
		f->Tick(this);
		tForce += f->Get();
	}

	mPosition += dt * mVelocity;
	mAcceleration = tForce / mMass;
	mVelocity += dt * mAcceleration;
}

VerletSystem::VerletSystem(float dt, float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration) :
	System(mass, position, velocity, acceleration),
	mLastPosition(position - velocity * dt),
	mFixedDeltaTime(dt)
{}

VerletSystem::VerletSystem(float dt, float mass) :
	System(mass), mLastPosition(mPosition), mFixedDeltaTime(dt)
{}

void VerletSystem::Tick(float dt)
{
	assert(dt == mFixedDeltaTime);
	glm::vec3 tForce(0.f);
	for (auto& f: mForces)
	{
		f->Tick(this);
		tForce += f->Get();
	}

	glm::vec3 tmp = mPosition;
	mAcceleration = tForce / mMass;
	mPosition 	  = 2.f * mPosition - mLastPosition + (mFixedDeltaTime * mFixedDeltaTime) * mAcceleration;
	mLastPosition = tmp;
	mVelocity     = (mPosition - mLastPosition) / mFixedDeltaTime;
}

StaticSystem::StaticSystem(glm::vec3 position) : System(0, position, glm::vec3(0.f), glm::vec3(0.f)) {}
