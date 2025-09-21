#include "buffer.h"

#include <glad/glad.h>

#include <iostream>

//////////////////////////////////// VERTEX BUFFER ///////////////////////////////////////

std::shared_ptr<vertex_buffer> vertex_buffer::create(const float* vertices, size_t vertex_buffer_size)
{
	return std::make_shared<vertex_buffer>(vertices, vertex_buffer_size);
}

vertex_buffer::vertex_buffer(const float* vertices, size_t vertex_buffer_size)
	: m_renderer_id(0), m_vertices(vertices), m_buffer_size(vertex_buffer_size)
{
	//std::cout << "vbo: element_count(float) [" << m_buffer_size / sizeof(float) << "]" << std::endl;
	//std::cout << "vbo: buffer_size [" << m_buffer_size << "]\n" << std::endl;

	glGenBuffers(1, &m_renderer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	glBufferData(GL_ARRAY_BUFFER, m_buffer_size, m_vertices, GL_STATIC_DRAW);
}

vertex_buffer::~vertex_buffer()
{
	glDeleteBuffers(1, &m_renderer_id);
}

void vertex_buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void vertex_buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////// INDEX BUFFER ///////////////////////////////////////

std::shared_ptr<index_buffer> index_buffer::create(uint32_t* indices, size_t index_buffer_size)
{
	return std::make_shared<index_buffer>(indices, index_buffer_size);
}

index_buffer::index_buffer(uint32_t* indices, size_t index_buffer_size)
	: m_indices(indices), m_count(index_buffer_size / sizeof(uint32_t)), m_buffer_size(index_buffer_size)
{
	//std::cout << "ibo: index_count [" << m_count << "]" << std::endl;
	//std::cout << "ibo: index_buffer_size [" << m_buffer_size << "]\n" << std::endl;

	glGenBuffers(1, &m_renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_buffer_size, m_indices, GL_STATIC_DRAW);
}

index_buffer::~index_buffer()
{
	glDeleteBuffers(1, &m_renderer_id);
}

void index_buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void index_buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}