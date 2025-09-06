#include "entity_object.h"

#include "barebones/core/core.h"

entity_object::entity_object(const mesh& mesh, const char* shader_filepath)
{
	if (shader_filepath == nullptr || std::strlen(shader_filepath) == 0)
		entity_material = material::instantiate(LIT_SHADER_SOURCE);
	else
		entity_material = material::instantiate(shader_filepath);

	entity_vertex_array = vertex_array::create();

	entity_vertex_buffer = vertex_buffer::create(mesh.get_vertices().data(), mesh.get_vertex_size());
	entity_vertex_buffer->set_layout({ { shader_data_type::Float3, "aPos" }, { shader_data_type::Float3, "aNormal" } });

	entity_vertex_array->add_vertex_buffers(entity_vertex_buffer);

	entity_index_buffer = index_buffer::create(mesh.get_indices().data(), mesh.get_indices_count());
	entity_vertex_array->set_index_buffer(entity_index_buffer);

	model_matrix = glm::mat4(1.0f);
	entity_material->set_model_matrix(&model_matrix[0][0]);
}

void entity_object::update_model_matrix()
{
	model_matrix = glm::translate(glm::mat4(1.0f), position);

	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model_matrix = glm::scale(model_matrix, scale);

	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
	normal = glm::normalize(normal_matrix * base_normal);

	entity_material->set_model_matrix(&model_matrix[0][0]);
}