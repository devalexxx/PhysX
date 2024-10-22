//
// Created by Alex on 15/10/2024.
//

#include <fmt/base.h>

#include <cstdlib>
#include <sstream>
#include <fstream>

#include <PhysX/Utils.h>
#include <PhysX/System.h>

int main(int /* argc */, char** /* argv */)
{
    float g = 10;
    float m = 80;

    glm::vec3 p0(0, 4000, 0);
    glm::vec3 v0(50.f, 0, 0);
	glm::vec3 a0(0.f);

    //	float rho = 1.2;
    //	float A   = 1.9;
    //	float Cd  = 1.2;
    //  float coef = should be 0.5 * rho * A * Cd
    //  But work with fg / glm::length(glm::vec3(0, 50, 0)) ** 2)
    //	float coef = -.32f;
    float coef = -(m * g) / (50.f * 50.f);

    float dt = 0.3;
    float tt = 100;
    float t  = 0;

    std::stringstream os;

	VerletSystem vs(dt, m, p0, v0, a0);
	vs.AddForce(Draft(glm::vec3(coef)));
	vs.AddForce(Gravity(g));

	os << fmt::format("{:.2f} {} {} {}\n", t, vs.GetPosition(), vs.GetVelocity(), vs.GetAcceleration());

    while (t < tt)
    {
        t += dt;
		vs.Tick(dt);
		os << fmt::format("{:.2f} {} {} {}\n", t, vs.GetPosition(), vs.GetVelocity(), vs.GetAcceleration());
    }

    {
        std::ofstream out("out.txt");
        if (out.is_open())
        {
            out << os.str();
        }
        else
        {
            fmt::print("Failed to write \"vout.txt\" file.");
        }
    }

    return EXIT_SUCCESS;
}