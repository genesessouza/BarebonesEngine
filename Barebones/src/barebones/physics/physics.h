#pragma once

#include <glm/ext/vector_float3.hpp>

class physics
{
public:
	static glm::vec3 gravity() { return glm::vec3(0.0f, -9.81f, 0.0f); }
};