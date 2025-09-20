#pragma once

#include "barebones/core/application.h"
#include "barebones/core/barebones/entity/entity_object.h"
#include "barebones/core/barebones/entity/light.h"
#include "barebones/core/barebones/camera/perspective_camera.h"
#include "core/frame_buffer.h"

#include <glm/ext/matrix_float4x4.hpp>

class renderer
{
public:
	renderer(glm::vec4 color);
	~renderer();

	void begin_scene(perspective_camera* camera, light* scene_light);
	void end_scene();

	void submit(const entity_object* entity_obj, const glm::mat4& light_space_matrix, const frame_buffer* fbo);
	void submit_depth(const entity_object* entity_obj, const glm::mat4& light_view, const glm::mat4& light_proj, const glm::mat4& light_space_pos);
private:
	void draw(const entity_object* entity_obj);
private:
	struct scene_data
	{
		glm::mat4 projection_matrix;
		glm::mat4 view_matrix;

		light* scene_light;
		perspective_camera* scene_camera;
	};

	static scene_data* m_scene_data;
};
