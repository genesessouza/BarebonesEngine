#include "gizmo.h"

#include <numbers>
#include <iostream>

void gizmo::create_arrow(glm::vec3 dir, glm::vec3 color)
{
	arrow arrow{};
	arrow.color = color;
	arrow.dir = dir;

	float vertices[] =
	{
		0.0f, 0.0f, 0.0f,
		dir.x, dir.y, dir.z
	};

	arrow.vao = vertex_array::create();
	arrow.vbo = vertex_buffer::create(vertices, sizeof(vertices));

	arrow.vbo->set_layout({ { shader_data_type::Float3, "aPos" } });
	arrow.vao->add_vertex_buffers(arrow.vbo);

	std::vector<float> coneVertices;
	generate_cone(coneVertices, 0.05f, 0.15f, 16);
	arrow.coneVertexCount = coneVertices.size() / 3;

	arrow.cone_vao = vertex_array::create();
	arrow.cone_vbo = vertex_buffer::create(coneVertices.data(), coneVertices.size() * sizeof(float));

	arrow.cone_vbo->set_layout({ { shader_data_type::Float3, "aPos" } });
	arrow.cone_vao->add_vertex_buffers(arrow.cone_vbo);

	arrows.push_back(arrow);
}

void gizmo::generate_cone(std::vector<float>& vertices, float radius, float height, int segments)
{
	vertices.clear();

	glm::vec3 tip(0, 0, height);
	glm::vec3 baseCenter(0, 0, 0);

	const float TWO_PI = 2.0f * std::numbers::pi_v<float>;

	for (int i = 0; i < segments; i++)
	{
		float theta1 = (float)i / segments * 2.0f * TWO_PI;
		float theta2 = (float)(i + 1) / segments * 2.0f * TWO_PI;

		glm::vec3 p1(radius * cos(theta1), radius * sin(theta1), 0);
		glm::vec3 p2(radius * cos(theta2), radius * sin(theta2), 0);

		// Lado
		vertices.insert(vertices.end(), { tip.x, tip.y, tip.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z });

		// Base
		vertices.insert(vertices.end(), { baseCenter.x, baseCenter.y, baseCenter.z, p2.x, p2.y, p2.z, p1.x, p1.y, p1.z });
	}
}

void gizmo::render(const glm::mat4& view, const glm::mat4& proj)
{
	get_material()->bind();

	for (auto& arrow : arrows)
	{
		get_material()->set_model_matrix(&model_matrix[0][0]);
		get_material()->get_shader()->define_mat4("u_view", &view[0][0]);
		get_material()->get_shader()->define_mat4("u_projection", &proj[0][0]);
		get_material()->get_shader()->define_vec3("u_color", arrow.color);

		arrow.vao->bind();
		glDrawArrays(GL_LINES, 0, 2);

		glm::vec3 endPos = position + arrow.dir;
		glm::mat4 coneModel = glm::translate(model_matrix, arrow.dir);

		if (arrow.dir == glm::vec3(1, 0, 0)) coneModel *= glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(0, 1, 0));
		if (arrow.dir == glm::vec3(0, 1, 0)) coneModel *= glm::rotate(glm::mat4(1.0f), -glm::half_pi<float>(), glm::vec3(1, 0, 0));

		get_material()->get_shader()->define_mat4("u_model", &coneModel[0][0]);

		arrow.cone_vao->bind();
		glDrawArrays(GL_TRIANGLES, 0, arrow.coneVertexCount);
	}
}

bool gizmo::intersect_ray_cone(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& coneTip,
	const glm::vec3& coneDir, float height, float radius, float& tHit)
{
	glm::vec3 co = rayOrigin - coneTip;

	float cos2 = (height * height) / (height * height + radius * radius);
	float dv = glm::dot(rayDir, coneDir);
	float co_v = glm::dot(co, coneDir);

	float a = dv * dv - cos2;
	float b = 2.0f * (dv * co_v - glm::dot(rayDir, co) * cos2);
	float c = co_v * co_v - glm::dot(co, co) * cos2;

	float disc = b * b - 4 * a * c;

	if (disc < 0.0f) return false;

	float t0 = (-b - sqrt(disc)) / (2 * a);
	float t1 = (-b + sqrt(disc)) / (2 * a);

	tHit = (t0 > 0) ? t0 : t1;

	if (tHit < 0) return false;

	glm::vec3 hitPoint = rayOrigin + rayDir * tHit;

	float h = glm::dot(hitPoint - coneTip, coneDir);

	return h >= 0.0f && h <= height;
}

gizmo_axis gizmo::pick_axis(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
{
	float tMin = std::numeric_limits<float>::max();
	gizmo_axis picked = gizmo_axis::none;

	float tHit;

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(1, 0, 0), 1.0f, 0.1f, tHit))
	{
		if (tHit < tMin)
		{
			tMin = tHit; picked = gizmo_axis::x;
			arrows[0].color = glm::vec3(1, 1, 1);
		}
	}

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(0, 1, 0), 1.0f, 0.7f, tHit))
	{
		if (tHit < tMin)
		{
			tMin = tHit; picked = gizmo_axis::y;

			arrows[1].color = glm::vec3(1, 1, 1);
		}
	}

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(0, 0, 1), 1.0f, 0.1f, tHit))
	{
		if (tHit < tMin)
		{
			tMin = tHit; picked = gizmo_axis::z;
			arrows[2].color = glm::vec3(1, 1, 1);
		}
	}

	selected = picked;
	return picked;
}

void gizmo::handle_mouse_pick(const glm::mat4& view, const glm::mat4& projection, const glm::vec2& mouseNDC, const glm::vec3& cameraPos)
{
	glm::vec4 rayClip(mouseNDC.x, mouseNDC.y, -1.0f, 1.0f);
	glm::vec4 rayEye = glm::inverse(projection) * rayClip;

	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

	glm::vec3 rayDir = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
	glm::vec3 rayOrigin = cameraPos;

	float tMin = std::numeric_limits<float>::max();
	gizmo_axis picked = gizmo_axis::none;
	float tHit;

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(1, 0, 0), 1.0f, 0.1f, tHit))
	{
		if (tHit < tMin)
			tMin = tHit; picked = gizmo_axis::x;
	}

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(0, 1, 0), 1.0f, 0.1f, tHit))
	{
		if (tHit < tMin)
			tMin = tHit; picked = gizmo_axis::y;
	}

	if (intersect_ray_cone(rayOrigin, rayDir, position, glm::vec3(0, 0, 1), 1.0f, 0.1f, tHit))
	{
		if (tHit < tMin)
			tMin = tHit; picked = gizmo_axis::z;
	}

	selected = picked;

	if (picked != gizmo_axis::none)
	{
		//set_position(glm::vec3(mouseNDC.x, mouseNDC.y, 0));
		//std::cout << "Picked axis: " << (picked == gizmo_axis::x ? "X" : picked == gizmo_axis::y ? "Y" : "Z") << std::endl;
	}
}
