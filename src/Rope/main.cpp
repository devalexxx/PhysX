//
// Created by Alex on 22/10/2024.
//

#include <PhysX/Utils.h>
#include <PhysX/System.h>
#include <PhysX/Constraint.h>

#include <sstream>
#include <fstream>
#include <filesystem>

int main(int /* argc */, char** /* argv */)
{
	namespace fs = std::filesystem;
	fs::create_directory("asset");
	fs::create_directory("asset/bridge");

	float g = 10;
	float m = 1;

	glm::vec3 p0(0, 20, 0);
	glm::vec3 v0(0, 0, 0);
	glm::vec3 a0(0.f);

	float coef = -(m * g) / (50.f * 50.f);

	float k = 10.f;
	glm::vec3 lanchor(-2, 5, 0);
	glm::vec3 ranchor( 2, 5, 0);
	float length = 2;

	float dt = 0.3;
	float tt = 1000;
	float t  = 0;

	std::stringstream os;

	StaticSystem lss(lanchor);
	StaticSystem rss(ranchor);

	VerletSystem vs(dt, m, p0, v0, a0);
	vs.AddForce(Gravity(g));
	vs.AddForce(Draft(glm::vec3(coef)));

	Rope lrp(k, length, lss, vs);
	Rope rrp(k, length, vs, rss);

	os << fmt::format("{:.2f} {} {} {}\n", t, vs.GetPosition(), vs.GetVelocity(), vs.GetAcceleration());

	while (t < tt)
	{
		t += dt;

		// Compute force applied by rope
		lrp.Tick(dt);
		rrp.Tick(dt);

		// Compute new parameter of the system
		vs.Tick(dt);

		os << fmt::format("{:.2f} {} {} {}\n", t, vs.GetPosition(), vs.GetVelocity(), vs.GetAcceleration());
	}

	{
		std::ofstream out("asset/bridge/out.txt");
		if (out.is_open())
		{
			out << os.str();
		}
		else
		{
			fmt::print("Failed to write \"vout.txt\" file.");
		}
	}

	return 0;
}