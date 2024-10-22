//
// Created by Alex on 22/10/2024.
//

#ifndef PHYSX_UTILS_H
#define PHYSX_UTILS_H

#include <glm/glm.hpp>
#include <fmt/format.h>

template<>
struct fmt::formatter<glm::vec3> : formatter<std::string>
{
		auto format(const glm::vec3& vec, format_context& ctx) const
		{
			auto fmt = fmt::format("{:.2f} {:.2f} {:.2f}", vec.x, vec.y, vec.z);
			return formatter<std::string>::format(fmt, ctx);
		}
};

#endif
