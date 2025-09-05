#include "glfw_window.h"

#include "../barebones/events/event_queue.h"
#include "../barebones/events/key_event.h"
#include "../barebones/events/mouse_event.h"

#include <glm/ext/vector_float2.hpp>

#include <iostream>
#include "../barebones/events/window_event.h"


glfw_window* glfw_window::create(const window_properties& props)
{
	glfw_window* window = new glfw_window(props);
	return window;
}

glfw_window::glfw_window(const window_properties& properties)
{
	init(properties);
}

glfw_window::~glfw_window()
{
	shutdown();
}

static bool s_glfw_initialized = false;
void glfw_window::init(const window_properties& props)
{
	m_data.title = props.title;
	m_data.width = props.width;
	m_data.height = props.height;

	if (!s_glfw_initialized)
	{
		int glfw_status_code = glfwInit();

		if (glfw_status_code == -1)
			std::cout << "Could not initialize GLFW!" << std::endl;

		s_glfw_initialized = true;
	}

	m_window = glfwCreateWindow(props.width, props.height, m_data.title.c_str(), nullptr, nullptr);

	if (!m_window)
	{
		std::cout << "Failed to create GLFW Window!" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	// -------------------------------- GLAD ---------------------------------

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization error!" << std::endl;
		return;
	}

	glfwSetWindowUserPointer(m_window, &m_data);

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);
			event_queue::instance().push(std::make_unique<window_closed>());
		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			if (action == GLFW_PRESS)
				event_queue::instance().push(std::make_unique<key_pressed>(key));
			else if (action == GLFW_RELEASE)
				event_queue::instance().push(std::make_unique<key_released>(key));
			else if (action == GLFW_REPEAT)
				event_queue::instance().push(std::make_unique<key_held>(key));
		});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(window);

			if (action == GLFW_PRESS)
				event_queue::instance().push(std::make_unique<mouse_clicked>(button));
			else if (action == GLFW_RELEASE)
				event_queue::instance().push(std::make_unique<mouse_released>(button));
		});
}

void glfw_window::on_update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void glfw_window::set_vsync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.vsync = enabled;
}

bool glfw_window::is_vsync() const
{
	return m_data.vsync;
}

glm::vec2 glfw_window::get_mouse_ndc() const
{
	double xpos, ypos;
	int width, height;

	glfwGetCursorPos(m_window, &xpos, &ypos);
	glfwGetWindowSize(m_window, &width, &height);

	float ndc_x = (2.0f * static_cast<float>(xpos)) / static_cast<float>(width) - 1.0f;
	float ndc_y = 1.0f - (2.0f * static_cast<float>(ypos)) / static_cast<float>(height);

	return glm::vec2(ndc_x, ndc_y);
}

void glfw_window::shutdown()
{
	if (m_window)
		glfwDestroyWindow(m_window);
}