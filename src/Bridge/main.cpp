//
// Created by Alex on 22/10/2024.
//

#include <PhysX/Utils.h>
#include <PhysX/System.h>

#include <sstream>
#include <fstream>
#include <filesystem>

int main(int /* argc */, char** /* argv */)
{
	namespace fs = std::filesystem;
	fs::create_directory("asset");
	fs::create_directory("asset/bridge");

	float g = 10;
	float m = 10;

	glm::vec3 p0(0, 0, 0);
	glm::vec3 v0(0, 0, 0);
	glm::vec3 a0(0.f);

	float coef = -(m * g) / (50.f * 50.f);

	float k = 1.f;
	glm::vec3 anchor(0, 5, 0);
	float length = glm::distance(p0, anchor);

	float dt = 0.1;
	float tt = 100;
	float t  = 0;

	std::stringstream os;

	VerletSystem vs(dt, m, p0, v0, a0);
	vs.AddForce(Gravity(g));
	vs.AddForce(Draft(glm::vec3(coef)));
	vs.AddForce(Rope(k, length, anchor));

	os << fmt::format("{:.2f} {} {} {}\n", t, vs.GetPosition(), vs.GetVelocity(), vs.GetAcceleration());

	while (t < tt)
	{
		t += dt;
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