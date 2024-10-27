//
// Created by Alex on 21/10/2024.
//

#ifndef PHYSX_SYSTEM_H
#define PHYSX_SYSTEM_H

#include <PhysX/Force.h>

#include <glm/glm.hpp>

#include <vector>

template<typename T>
concept IsForce = std::is_base_of_v<Force, T>;

class System
{
	public:
		System(float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration);
		System(float mass);

		template<IsForce T>
		void AddForce(T force)
		{
			mForces.push_back(std::make_unique<T>(std::move(force)));
		}
		[[nodiscard]] float GetMass() const;
		[[nodiscard]] const glm::vec3& GetPosition() const;
		[[nodiscard]] const glm::vec3& GetVelocity() const;
		[[nodiscard]] const glm::vec3& GetAcceleration() const;

		virtual void Tick(float dt) = 0;

	protected:


	protected:
		std::vector<std::unique_ptr<Force>> mForces;
		float mMass;

		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::vec3 mAcceleration;
};

class EulerSystem : public System
{
	public:
		using System::System;

		void Tick(float dt) override;
};

class VerletSystem : public System
{
	public:
		VerletSystem(float dt, float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration);
		VerletSystem(float dt, float mass);

		void Tick(float dt) override;

	private:
		glm::vec3 mLastPosition;
		float 	  mFixedDeltaTime;
};

class StaticSystem : public System
{
	public:
		explicit StaticSystem(glm::vec3 position);

		void Tick(float) override {};
};

#endif
