//
// Created by Alex on 26/10/2024.
//

#ifndef PHYSX_CONSTRAINT_H
#define PHYSX_CONSTRAINT_H

#include <PhysX/Force.h>

class Constraint
{
	public:
		virtual void Tick(float dt) = 0;
};

class Rope : public Constraint
{
	class LForce;
	class RForce;

	public:
		Rope(float k, float length, System& left, System& right);

		void Tick(float dt) override;

	private:
		float mK;
		float mLength;

		glm::vec3 mLeftForce;
		glm::vec3 mRightForce;

		const System& mLeft;
		const System& mRight;

	private:
		class LForce : public Force
		{
			public:
				explicit LForce(const Rope& rope);
				void Tick(System* s) override;

			private:
				const Rope& mRope;
		};

		class RForce : public Force
		{
			public:
				explicit RForce(const Rope& rope);
				void Tick(System* s) override;

			private:
				const Rope& mRope;
		};
};

#endif
