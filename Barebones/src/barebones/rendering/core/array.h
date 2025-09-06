#pragma once

#include "buffer.h"

#include <cstdint>
#include <memory>
#include <vector>

class vertex_array
{
public:
	vertex_array();
	static std::shared_ptr<vertex_array> create();
	~vertex_array();

	void bind() const;
	void unbind() const;

	const uint32_t get_vertex_array_id() const { return renderer_id; }
	std::shared_ptr<vertex_array> get_vertex_buffer_object() const { return vertex_array_object; }

	void add_vertex_buffers(const std::shared_ptr<vertex_buffer>& vertex_buffer);
	void set_index_buffer(const std::shared_ptr<index_buffer>& index_buffer);

	const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() { return vertex_buffer_objects; }
	const std::shared_ptr<index_buffer>& get_index_buffer() { return index_buffer_object; }
private:
	uint32_t renderer_id;
	std::shared_ptr<vertex_array> vertex_array_object;
	std::vector<std::shared_ptr<vertex_buffer>> vertex_buffer_objects;
	std::shared_ptr<index_buffer> index_buffer_object;
};