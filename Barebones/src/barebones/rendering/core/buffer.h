#pragma once

#include "buffer_layout.h"

#include <cstdint>
#include <memory>

class vertex_buffer
{
public:
	vertex_buffer(const float* vertices, size_t size);
	static std::shared_ptr<vertex_buffer> create(const float* vertices, size_t size);
	~vertex_buffer();

	void bind() const;
	void unbind() const;

	void set_layout(const buffer_layout& layout) { this->layout = layout; }
	const buffer_layout& get_layout() const { return this->layout; }

	uint32_t get_vertex_buffer_id() const { return m_renderer_id; }
	std::shared_ptr<vertex_buffer> get_vertex_buffer_object() const { return vertex_buffer_object; }

	const float* get_vertices() const { return m_vertices; }
	size_t get_buffer_size() const { return m_buffer_size; }
private:
	uint32_t m_renderer_id;
	std::shared_ptr<vertex_buffer> vertex_buffer_object;

	const float* m_vertices;
	size_t m_buffer_size;
	buffer_layout layout;
};

class index_buffer
{
public:
	index_buffer(uint32_t* indices, size_t count);
	static std::shared_ptr<index_buffer> create(uint32_t* indices, size_t count);
	~index_buffer();

	void bind() const;
	void unbind() const;

	uint32_t get_index_buffer_id() const { return m_renderer_id; }
	std::shared_ptr<index_buffer> get_index_buffer_object() const { return index_buffer_object; }

	uint32_t* get_indices() const { return m_indices; }

	size_t get_count() const { return m_count; }
private:
	uint32_t m_renderer_id;
	std::shared_ptr<index_buffer> index_buffer_object;

	uint32_t* m_indices;
	size_t m_count;
	size_t m_buffer_size;
};
