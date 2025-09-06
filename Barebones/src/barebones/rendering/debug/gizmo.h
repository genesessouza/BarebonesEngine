#include "barebones/core/barebones/entity/entity_object.h"

#include <glad/glad.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

struct arrow 
{
	std::shared_ptr<vertex_array> vao;
	std::shared_ptr<vertex_buffer> vbo;

	std::shared_ptr<vertex_array> cone_vao;
	std::shared_ptr<vertex_buffer> cone_vbo;

	int coneVertexCount;

	glm::vec3 color;
	glm::vec3 dir;
};

enum class gizmo_axis 
{
	none,
	x,
	y,
	z
};

class gizmo : public entity_object
{
public:
	gizmo(const char* shader_filepath)
		: entity_object(shader_filepath)
	{
		create_arrow(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0));
		create_arrow(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		create_arrow(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1));
	}

	void render(const glm::mat4& view, const glm::mat4& proj);

	void handle_mouse_pick(const glm::mat4& view, const glm::mat4& projection, const glm::vec2& mouseNDC, const glm::vec3& cameraPos);
private:
	void create_arrow(glm::vec3 dir, glm::vec3 color);
	void generate_cone(std::vector<float>& vertices, float radius, float height, int segments);

	bool intersect_ray_cone(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& coneTip,
		const glm::vec3& coneDir, float height, float radius, float& tHit);

	gizmo_axis pick_axis(const glm::vec3& rayOrigin, const glm::vec3& rayDir);

	gizmo_axis get_selected_axis() const { return selected; }
private:
	std::vector<arrow> arrows;
	gizmo_axis selected = gizmo_axis::none;
};