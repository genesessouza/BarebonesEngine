#pragma once

#include "barebones/core/application.h"
#include "barebones/core/barebones/entity/entity_object.h"
#include "barebones/core/barebones/entity/light.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

#include <glm/ext/matrix_float4x4.hpp>

class renderer
{
public:
	renderer(glm::vec4 color);
	~renderer();

	void begin_scene(const perspective_camera* camera, light* scene_light);
	void end_scene();

	void submit(const entity_object* entity_obj, const perspective_camera* scene_camera, const glm::mat4& transform = glm::mat4(1.0f));
private:
	void draw(const entity_object* entity_obj);
private:
	struct scene_data
	{
		glm::mat4 projection_matrix;
		glm::mat4 view_matrix;

		light* scene_light;
	};

	static scene_data* m_scene_data;
};
