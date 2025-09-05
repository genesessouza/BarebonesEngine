#pragma once

#include "../barebones/events/event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <string>

struct window_properties
{
	std::string title;
	unsigned int width;
	unsigned int height;

	window_properties(const std::string& title = "TRASH! Engine",
		unsigned int width = 800,
		unsigned int height = 600)
		: title(title), width(width), height(height)
	{
	}
};

class glfw_window
{
public:
	using event_callback_fn = std::function<void(event&)>;

	static glfw_window* create(const window_properties & = window_properties());

	glfw_window(const window_properties& properties);
	~glfw_window();

	void on_update();

	inline unsigned int get_width() const { return m_data.width; }
	inline unsigned int get_height() const { return m_data.height; }

	inline void set_event_callback(const event_callback_fn& callback) { m_data.event_callback = callback; }
	void set_vsync(bool enabled);
	bool is_vsync() const;

	inline GLFWwindow* get_native_window() const { return m_window; }
	glm::vec2 get_mouse_ndc() const;

	void shutdown();
private:
	void init(const window_properties& props);
private:
	struct window_data
	{
		std::string title = "";
		unsigned int width = 0;
		unsigned int height = 0;
		bool vsync = false;

		event_callback_fn event_callback;
	};
private:
	GLFWwindow* m_window;
	bool m_vsync;

	window_data m_data;
};