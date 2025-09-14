#include "shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <fstream>
#include <iostream>

shader_source shader::parse_file() const
{
	std::ifstream stream(m_shader_filepath);

	std::string line;
	std::stringstream ss[2];

	shader_type type = shader_type::none;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = shader_type::vertex;
			else if (line.find("fragment") != std::string::npos)
				type = shader_type::fragment;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

uint32_t shader::compile(unsigned int type, const std::string& shader_source)
{
	unsigned int shaderId = glCreateShader(type);
	const char* source = shader_source.c_str();

	glShaderSource(shaderId, 1, &source, nullptr);
	glCompileShader(shaderId);

	int compilationResult;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationResult);

	if (compilationResult == GL_FALSE)
	{
		int messageLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);
		char* message = (char*)_malloca(messageLength * sizeof(char));

		glGetShaderInfoLog(shaderId, messageLength, &messageLength, message);

		const char* shaderType;
		if (type == GL_VERTEX_SHADER)
			shaderType = "vertex shader";
		else
			shaderType = "fragment shader";

		glDeleteShader(shaderId);
		std::cout << "Failed to compile " << shaderType << "!" << std::endl;
		std::cout << message << std::endl;

		return 0;
	}

	return shaderId;
}

void shader::create(const std::string& vertex_source, const std::string& fragment_source)
{
	m_renderer_id = glCreateProgram();

	unsigned int vertex_shader = compile(GL_VERTEX_SHADER, vertex_source);
	unsigned int fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_source);

	glAttachShader(m_renderer_id, vertex_shader);
	glAttachShader(m_renderer_id, fragment_shader);

	glLinkProgram(m_renderer_id);

	uint32_t is_linked = 0;
	glGetProgramiv(m_renderer_id, GL_LINK_STATUS, (int*)&is_linked);
	if (is_linked == GL_FALSE)
	{
		int message_length = 0;
		glGetProgramiv(m_renderer_id, GL_INFO_LOG_LENGTH, &message_length);

		char* message = (char*)_malloca(message_length * sizeof(char));
		glGetProgramInfoLog(m_renderer_id, message_length, &message_length, &message[0]);

		glDeleteProgram(m_renderer_id);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		std::cout << "Link compilation error!" << std::endl;
		std::cout << message << std::endl;

		return;
	}
	glValidateProgram(m_renderer_id);

	glDetachShader(m_renderer_id, vertex_shader);
	glDetachShader(m_renderer_id, fragment_shader);
}

std::shared_ptr<shader> shader::instantiate(const char* m_shader_filepath)
{
	return std::make_shared<shader>(m_shader_filepath);
}

shader::shader(const std::string& m_shader_filepath) : m_shader_filepath(m_shader_filepath)
{
	shader_source source = parse_file();

	create(source.vertex_source, source.fragment_source);
	glUseProgram(m_renderer_id);
}

shader::~shader()
{
	if (m_renderer_id != 0) {
		glDeleteProgram(m_renderer_id);
		m_renderer_id = 0;
	}
}

void shader::get_shader_info_log(const std::string& uniform_name) const
{
	int message_length;
	glGetShaderiv(m_renderer_id, GL_INFO_LOG_LENGTH, &message_length);
	char* message = (char*)_malloca(message_length * sizeof(char));

	glGetShaderInfoLog(m_renderer_id, message_length, &message_length, message);
	std::cout << "Uniform <" << uniform_name.c_str() << "> not found!: " << message << std::endl;
}

void shader::bind() const
{
	glUseProgram(m_renderer_id);
}

void shader::unbind() const
{
	glUseProgram(0);
}

int shader::get_uniform_location(const std::string& name)
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	int location = glGetUniformLocation(m_renderer_id, name.c_str());
	if (location == -1)
		get_shader_info_log(name);

	m_uniform_location_cache[name] = location;
	return location;
}

void shader::define_mat4(const std::string& name, const float* matrix)
{
	int location = get_uniform_location(name);

	glUseProgram(m_renderer_id);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

void shader::define_float(const std::string& name, const float value)
{
	int location = get_uniform_location(name);

	glUseProgram(m_renderer_id);
	glUniform1f(location, value);
}

void shader::define_bool(const std::string& name, const bool value)
{
	int location = get_uniform_location(name);

	glUseProgram(m_renderer_id);
	glUniform1i(location, value);
}

void shader::define_vec3(const std::string& name, const glm::vec3 vector)
{
	int location = get_uniform_location(name);

	glUseProgram(m_renderer_id);
	glUniform3fv(location, 1, glm::value_ptr(glm::vec3(vector.x, vector.y, vector.z)));
}

void shader::define_vec4(const std::string& name, const glm::vec4& vector)
{
	int location = get_uniform_location(name);

	glUseProgram(m_renderer_id);
	glUniform4fv(location, 1, glm::value_ptr(vector));
}

void shader::get_float(const std::string& name)
{
	int location = get_uniform_location(name);

	float value;

	glUseProgram(m_renderer_id);
	glGetUniformfv(m_renderer_id, location, &value);
}

glm::vec3 shader::get_vec3(const std::string& name)
{
	int location = get_uniform_location(name);

	float vector[3];

	glUseProgram(m_renderer_id);
	glGetUniformfv(m_renderer_id, location, vector);

	return glm::vec3(vector[0], vector[1], vector[2]);
}

glm::mat3 shader::get_mat3(const std::string& name)
{
	int location = get_uniform_location(name);

	float matrix[9];
	glUseProgram(m_renderer_id);
	glGetUniformfv(m_renderer_id, location, matrix);

	return glm::mat3(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8]);
}

glm::vec4 shader::get_vec4(const std::string& name)
{
	int location = get_uniform_location(name);

	float vector[4];

	glUseProgram(m_renderer_id);
	glGetUniformfv(m_renderer_id, location, vector);

	return glm::vec4(vector[0], vector[1], vector[2], vector[3]);
}

glm::mat4 shader::get_mat4(const std::string& name)
{
	int location = get_uniform_location(name.c_str());

	float matrix[16];

	glUseProgram(m_renderer_id);
	glGetUniformfv(m_renderer_id, location, matrix);

	return glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11], matrix[12], matrix[13], matrix[14], matrix[15]);
}