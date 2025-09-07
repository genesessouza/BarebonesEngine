#pragma once

#include "barebones/core/core.h"
#include "barebones/core/barebones/entity/mesh.h"

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <memory>
#include <string>
#include <cstdint>

struct shader_source
{
	std::string vertex_source;
	std::string fragment_source;
};

class shader
{
public:
	shader(const std::string& shader_filepath);
	static std::shared_ptr<shader> instantiate(const char* shader_filepath);
	~shader();

	void bind() const;
	void unbind() const;

	void get_shader_info_log(const std::string& uniform_name) const;

	int get_uniform_location(const std::string& name);

	void define_float(const std::string& name, const float value);
	void define_vec3(const std::string& name, const glm::vec3);
	void define_mat4(const std::string& name, const float* matrix);
	void define_vec4(const std::string& name, const glm::vec4&);

	void get_float(const std::string& name);

	glm::vec3 get_vec3(const std::string& name);
	glm::vec4 get_vec4(const std::string& name);

	glm::mat3 get_mat3(const std::string& name);
	glm::mat4 get_mat4(const std::string& name);

	// ------------------------------ EXPERIMENTAL -------------------------------- //
	
	void define_ubo(const std::string& blockName, const std::vector<sdf_primitive>& primitives) const;

	// ---------------------------------------------------------------------------- //

	uint32_t get_shader() const { return renderer_id; }
private:
	shader_source parse_file() const;
	uint32_t compile(unsigned int type, const std::string& source);
	void create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
private:
	enum class shader_type { none = -1, vertex = 0, fragment = 1 };
private:
	std::string shader_filepath;
	uint32_t renderer_id;

	mutable std::unordered_map<std::string, int> m_uniform_location_cache;
};
