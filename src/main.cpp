//
// Created by Alex on 15/10/2024.
//

#include <glm/glm.hpp>
#include <fmt/format.h>

#include <cstdlib>
#include <sstream>
#include <fstream>

template<>
struct fmt::formatter<glm::vec3> : formatter<std::string>
{
    auto format(const glm::vec3& vec, format_context& ctx) const
    {
        auto fmt = fmt::format("{:.2f} {:.2f} {:.2f}", vec.x, vec.y, vec.z);
        return formatter<std::string>::format(fmt, ctx);
    }
};

int main(int /* argc */, char** /* argv */)
{
    float g = 10;
    float m = 80;

    glm::vec3 p0(0, 4000, 0);
    glm::vec3 v0(200 / 3.6, 0, 0);

    glm::vec3 fg = glm::vec3(0, -1, 0) * m * g;
    glm::vec3 fd = glm::vec3(0, 0, 0);

    //	float rho = 1.2;
    //	float A   = 1.9;
    //	float Cd  = 1.2;
    //  float coef = should be 0.5 * rho * A * Cd
    //  But work with fg / glm::length(glm::vec3(0, 50, 0)) ** 2)
    //	float coef = -.32f;
    float coef = (fg / glm::pow(glm::length(glm::vec3(0, 50, 0)), 2.f)).y;

    fmt::print("{}\n", coef);

    glm::vec3 p = p0;
    glm::vec3 v = v0;
    glm::vec3 a;

    float t  = 0;
    float dt = 0.01;
    float tt = 100;

    std::stringstream vStream;
    std::stringstream pStream;
    std::stringstream aStream;

    while (t < tt)
    {
        if (float norm = glm::length(v); norm != 0)
            fd = coef * norm * v;
        else
            fd = glm::vec3(0, 0, 0);

        p += dt * v;
        a = (fg + fd) / m;
        v += dt * a;

        t += dt;

        vStream << fmt::format("{:.2f} {}\n", t, v);
        pStream << fmt::format("{:.2f} {}\n", t, p);
        aStream << fmt::format("{:.2f} {}\n", t, a);
    }

    {
        std::ofstream vOut("vout.txt");
        if (vOut.is_open())
        {
            vOut << vStream.str();
        }
        else
        {
            fmt::print("Failed to write \"vout.txt\" file.");
        }
    }

    {
        std::ofstream pOut("pout.txt");
        if (pOut.is_open())
        {
            pOut << pStream.str();
        }
        else
        {
            fmt::print("Failed to write \"pout.txt\" file.");
        }
    }

    {
        std::ofstream aOut("aout.txt");
        if (aOut.is_open())
        {
            aOut << vStream.str();
        }
        else
        {
            fmt::print("Failed to write \"aout.txt\" file.");
        }
    }

    return EXIT_SUCCESS;
}