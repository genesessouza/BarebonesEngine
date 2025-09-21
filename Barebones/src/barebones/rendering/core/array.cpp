#include "array.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

std::shared_ptr<vertex_array> vertex_array::create()
{
	return std::make_shared<vertex_array>();
}

vertex_array::vertex_array()
{
	glGenVertexArrays(1, &m_renderer_id);
	glBindVertexArray(m_renderer_id);
}

vertex_array::~vertex_array()
{
	glDeleteVertexArrays(1, &m_renderer_id);
}

void vertex_array::bind() const
{
	glBindVertexArray(m_renderer_id);
}

void vertex_array::unbind() const
{
	glBindVertexArray(0);
}

void vertex_array::add_vertex_buffers(const std::shared_ptr<vertex_buffer>& vertex_buffer)
{
	bind();
	vertex_buffer->bind();

	const auto& layout = vertex_buffer->get_layout();
	uint32_t index = 0;

	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.get_component_count(),
			GL_FLOAT,
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.get_stride(),
			(const void*)element.offset
		);
		index++;
	}

	vertex_buffer_objects.push_back(vertex_buffer);
}

void vertex_array::set_index_buffer(const std::shared_ptr<index_buffer>& index_buffer)
{
	bind();
	index_buffer->bind();

	index_buffer_object = index_buffer;
}