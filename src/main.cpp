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
    glm::vec3 v0(50.f, 0, 0);

    //	float rho = 1.2;
    //	float A   = 1.9;
    //	float Cd  = 1.2;
    //  float coef = should be 0.5 * rho * A * Cd
    //  But work with fg / glm::length(glm::vec3(0, 50, 0)) ** 2)
    //	float coef = -.32f;
    float coef = -(m * g) / (50.f * 50.f);

    glm::vec3 fg = glm::vec3(0, -1, 0) * m * g;
    glm::vec3 fd = coef * glm::length(v0) * v0;

    float dt = 0.3;
    float tt = 100;
    float t  = 0;

    std::stringstream vStream;
    std::stringstream pStream;
    std::stringstream aStream;

    glm::vec3 a   = (fg + fd) / m;
    glm::vec3 pdt = p0;
    glm::vec3 pt  = p0 + v0 * dt;
    glm::vec3 p   = pt;
    glm::vec3 v   = v0;

    vStream << fmt::format("{:.2f} {}\n", t, v0);
    pStream << fmt::format("{:.2f} {}\n", t, p0);
    aStream << fmt::format("{:.2f} {}\n", t, a);

    while (t < tt)
    {
        t += dt;

        pt  = p;
        p   = 2.f * pt - pdt + (dt * dt) * a;
        pdt = pt;
        v   = (p - pdt) / dt;

        if (float norm = glm::length(v); norm != 0)
            fd = coef * norm * v;
        else
            fd = glm::vec3(0, 0, 0);
        a = (fg + fd) / m;

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
            aOut << aStream.str();
        }
        else
        {
            fmt::print("Failed to write \"aout.txt\" file.");
        }
    }

    return EXIT_SUCCESS;
}