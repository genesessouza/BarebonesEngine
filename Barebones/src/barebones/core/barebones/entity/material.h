#pragma once

#include "barebones/rendering/shader/shader.h"

#include <memory>

class material
{
public:
	material(const char* shader_filepath = nullptr);
	static std::shared_ptr<material> instantiate(const char* shader_filepath = nullptr);
	~material();

	void bind() const;
	void unbind() const;

	void set_shader(const std::shared_ptr<shader>& material_shader) { m_shader = material_shader; }
	std::shared_ptr<shader> get_shader() const { return m_shader; }
	std::shared_ptr<shader> get_debug_shader() const { return m_debug_shader; }

	void set_color(const glm::vec4& color) { m_shader->define_vec4("u_color", color); }
	const glm::vec4& get_color(const std::string& uniform_name) const;

	void set_model_matrix(float* matrix) { m_shader->define_mat4("u_model", matrix); m_debug_shader->define_mat4("u_model", matrix); }
private:
	std::shared_ptr<shader> m_shader;
	std::shared_ptr<shader> m_debug_shader;
};