//
// Created by Alex on 21/10/2024.
//

#ifndef PHYSX_FORCE_H
#define PHYSX_FORCE_H

#include <glm/glm.hpp>

class System;

class Force
{
	public:
		Force();
		virtual ~Force() = default;

		virtual void Tick(System* s) = 0;

		[[nodiscard]] const glm::vec3& Get() const;

	protected:
		glm::vec3 mForce;
};

class Gravity : public Force
{
	public:
		Gravity(float gravity);

		void Tick(System* s) override;

	private:
		float mGravity;
};

class Draft : public Force
{
	public:
		Draft(glm::vec3 coef);

		void Tick(System* s) override;

	private:
		glm::vec3 mCoef;
};

class Rope : public Force
{
	public:
		Rope(float k, float length, glm::vec3 anchor);

		void Tick(System* s) override;

	private:
		float 	  mK;
		float     mLength;
		glm::vec3 mAnchor;
};

#endif
