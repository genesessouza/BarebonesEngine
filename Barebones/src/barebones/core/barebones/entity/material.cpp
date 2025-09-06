#include "material.h"

#include <iostream>

std::shared_ptr<material> material::instantiate(const char* shader_filepath)
{
	return std::make_shared<material>(shader_filepath);
}

material::material(const char* shader_filepath)
{
	m_shader = shader::instantiate(shader_filepath);
	m_debug_shader = shader::instantiate(DEBUG_SHADER_SOURCE);
}

material::~material()
{
}

void material::bind() const
{
	m_shader->bind();
}

void material::unbind() const
{
	m_shader->unbind();
}

const glm::vec4& material::get_color(const std::string& uniform_name) const
{
	return m_shader->get_vec4(uniform_name);
}