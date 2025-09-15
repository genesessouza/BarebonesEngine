#pragma once

#include <cstdint>

class frame_buffer
{
public:
	frame_buffer(int width, int height);

	int get_width() const { return m_width; }
	int get_height() const { return m_height; }

	uint32_t get_depth_texture() const { return m_depth_texture; }

	uint32_t get() const { return m_renderer_id; }
private:
	uint32_t m_renderer_id;
	uint32_t m_depth_texture;

	int m_width;
	int m_height;
};