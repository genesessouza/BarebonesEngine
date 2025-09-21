#pragma once

#include "barebones/rendering/shader/shader.h"

#include <memory>

class material
{
public:
	material(const char* m_shader_filepath = nullptr);
	static std::shared_ptr<material> instantiate(const char* m_shader_filepath = nullptr);
	~material();

	void bind() const;
	void unbind() const;

	void set_shader(const std::shared_ptr<shader>& material_shader) { m_shader = material_shader; }
	std::shared_ptr<shader> get_shader() const { return m_shader; }
	std::shared_ptr<shader> get_debug_shader() const { return m_debug_shader; }

	void set_color(const glm::vec4& color) { m_shader->define_vec4("u_color", color); }
	const glm::vec4& get_color(const std::string& uniform_name) const;

	void set_ambient(float ambient_strength) { m_shader->define_float("u_ambientMultiplier", ambient_strength); }
	void set_diffuse(float diffuse_strength) { m_shader->define_float("u_diffuseMultiplier", diffuse_strength); }
	void set_specular(float specular_strength) { m_shader->define_float("u_specularMultiplier", specular_strength); }
	void set_shadow_strength(float shadow_strength) { m_shader->define_float("u_shadowStrength", shadow_strength); }

	void set_model_matrix(float* matrix) { m_shader->define_mat4("u_model", matrix); m_debug_shader->define_mat4("u_model", matrix); }
	const glm::mat4& get_model_matrix() const { return m_shader->get_mat4("u_model"); }
private:
	std::shared_ptr<shader> m_shader;
	std::shared_ptr<shader> m_debug_shader;
};