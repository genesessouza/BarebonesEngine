#include "buffer.h"

#include <glad/glad.h>
#include <iostream>


//////////////////////////////////// VERTEX BUFFER ///////////////////////////////////////

std::shared_ptr<vertex_buffer> vertex_buffer::create(const float* vertices, uint32_t size)
{
	return std::make_shared<vertex_buffer>(vertices, size);
}

vertex_buffer::vertex_buffer(const float* vertices, uint32_t size)
	: renderer_id(0)
{
	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	this->vertices = vertices;
}

vertex_buffer::~vertex_buffer()
{
	glDeleteBuffers(1, &renderer_id);
}

void vertex_buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
}

void vertex_buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////// INDEX BUFFER ///////////////////////////////////////

std::shared_ptr<index_buffer> index_buffer::create(const uint32_t* indices, uint32_t count)
{
	return std::make_shared<index_buffer>(indices, count);
}

index_buffer::index_buffer(const uint32_t* indices, uint32_t count) : count(count)
{
	glGenBuffers(1, &renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	//std::cout << count << std::endl;
}

index_buffer::~index_buffer()
{
	glDeleteBuffers(1, &renderer_id);
}

void index_buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
}

void index_buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}