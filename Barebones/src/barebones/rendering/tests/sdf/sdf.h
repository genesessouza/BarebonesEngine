#pragma once

#include "barebones/core/barebones/entity/entity.h"

#include <glm/ext/matrix_transform.hpp>

class sdf
{
public:
	float dot_product_vec3(glm::vec3 v) { return glm::dot(v, v); } // dot_product

	float sdPlane(glm::vec3 point, glm::vec3 plane, float h)
	{
		// point must be normalized
		return glm::dot(point, glm::normalize(plane)) + h;
	}

	float sdf_to_circle(glm::vec3 point, glm::vec3 centre, float radius) { return length(centre - point) - radius; }

	float sdf_to_box(glm::vec3 point, glm::vec3 box, glm::vec3 size)
	{
		glm::vec3 offset = abs(point - box) - size;
		return glm::length(glm::max(offset, 0.0f)) + glm::min(glm::max(offset.x, glm::max(offset.y, offset.z)), 0.0f);
	}

	float sdf_to_scene(glm::vec3 point)
	{
		float dst_to_scene = 30;

		for (auto& scene_obj : scene_objects)
		{
		}
	}
private:
	std::vector<entity> scene_objects;
};