#include "entity_object.h"

#include "barebones/core/core.h"

#include <glad/glad.h>
#include <iostream>

entity_object::entity_object(const mesh& mesh)
{
	entity_vertex_array = vertex_array::create();

	entity_vertex_buffer = vertex_buffer::create(mesh.get_vertices().data(), mesh.get_vertex_buffer_size());
	entity_vertex_buffer->set_layout({ { shader_data_type::Float3, "aPos" }, { shader_data_type::Float3, "aNormal" } });

	entity_vertex_array->add_vertex_buffers(entity_vertex_buffer);

	entity_index_buffer = index_buffer::create(mesh.get_indices().data(), mesh.get_index_buffer_size());
	entity_vertex_array->set_index_buffer(entity_index_buffer);

	model_matrix = glm::mat4(1.0f);
}

void entity_object::set_position(const glm::vec3& position, const axis axis)
{
	if (axis == axis::local)
		m_position = m_orientation * position;
	else
		m_position = position;

	update_model_matrix();
}

void entity_object::set_rotation(const glm::vec3& rotation, const axis axis)
{
	m_rotation = glm::radians(rotation);

	glm::quat qx = glm::angleAxis(m_rotation.x, glm::vec3(1, 0, 0));
	glm::quat qy = glm::angleAxis(m_rotation.y, glm::vec3(0, 1, 0));
	glm::quat qz = glm::angleAxis(m_rotation.z, glm::vec3(0, 0, 1));
	glm::quat delta = qx * qy * qz;

	m_orientation = glm::normalize(delta);

	update_model_matrix();
}

void entity_object::update_model_matrix()
{
	glm::mat4 rotation_matrix = glm::mat4_cast(m_orientation);
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), m_scale);

	model_matrix = translation_matrix * rotation_matrix * scale_matrix;

	normal = glm::normalize(m_orientation * glm::vec3(0, 0, 1));

	entity_material->set_model_matrix(&model_matrix[0][0]);
}