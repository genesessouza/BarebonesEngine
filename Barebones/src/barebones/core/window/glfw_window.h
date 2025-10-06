#pragma once

#include "barebones/core/barebones/events/event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <string>

struct window_properties
{
	std::string title;
	float width;
	float height;

	window_properties(const std::string& title = "TRASH! Engine",
		float width = 800,
		float height = 600)
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

	inline float get_initial_width() const { return m_initial_width; }
	inline float get_initial_height() const { return m_initial_height; }

	inline float get_width() const { return m_data.width; }
	inline float get_height() const { return m_data.height; }

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
		float width = 0;
		float height = 0;
		bool vsync = false;

		event_callback_fn event_callback;
	};

	float m_initial_width;
	float m_initial_height;
public:
	window_data& get_data() { return m_data; }
private:
	GLFWwindow* m_window;
	bool m_vsync;

	window_data m_data;
};