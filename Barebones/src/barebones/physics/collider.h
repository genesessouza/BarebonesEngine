#pragma once

#include "barebones/core/barebones/entity/mesh.h"
#include "barebones/rendering/shader/shader.h"

struct wireframe
{
	mesh mesh;
	std::vector<std::pair<glm::vec3, glm::vec3>> lines;
	std::shared_ptr<shader> debug_shader;

	unsigned int vao = 0;
	unsigned int vbo = 0;
};

class collider
{
public:
	collider(mesh& mesh);
	~collider() = default;

	void draw_collider(const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const glm::mat4& model_matrix);
private:
	void create_wireframe();
	void set_wireframe_data();
private:
	wireframe extents_debug;
	glm::vec4 wireframe_color = glm::vec4(1.0f, 0.7f, 0.0f, 1.0f);
};