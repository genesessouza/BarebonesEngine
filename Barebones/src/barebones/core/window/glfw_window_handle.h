#pragma once

#include <GLFW/glfw3.h>

class glfw_window_handle {
public:
	explicit glfw_window_handle(GLFWwindow* w = nullptr) : m_w(w) {}

	~glfw_window_handle() { if (m_w) glfwDestroyWindow(m_w); }
	glfw_window_handle(const glfw_window_handle&) = delete;

	glfw_window_handle& operator=(const glfw_window_handle&) = delete;

	glfw_window_handle(glfw_window_handle&& o) noexcept : m_w(o.m_w) { o.m_w = nullptr; }

	glfw_window_handle& operator=(glfw_window_handle&& o) noexcept {
		if (this != &o) { if (m_w) glfwDestroyWindow(m_w); m_w = o.m_w; o.m_w = nullptr; }
		return *this;
	}

	GLFWwindow* get() { return m_w; }
private:
	GLFWwindow* m_w = nullptr;
};