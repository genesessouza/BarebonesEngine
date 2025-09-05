#include "application.h"

application* application::s_instance = nullptr;

application::application() : m_event_layer(new event_layer()), m_input_layer(new input_layer())
{
	if (s_instance)
	{
		std::cout << "Application already running!" << std::endl;
		return;
	}

	s_instance = this;

	m_window = std::unique_ptr<glfw_window>(glfw_window::create());
	m_window->set_vsync(false);

	m_event_layer = new event_layer();
	push_layer(m_event_layer);

	m_input_layer = new input_layer();
	push_layer(m_input_layer);
}

void application::push_layer(layer* layer)
{
	m_layer_stack.push_layer(layer);
	layer->on_attach();
}

void application::push_overlay(layer* overlay)
{
	m_layer_stack.push_overlay(overlay);
	overlay->on_attach();
}

application::~application()
{
}

void application::run()
{
	while (m_running)
	{
		float time = (float)glfwGetTime();
		delta_time = time - m_last_frame_time;
		m_last_frame_time = time;

		// FPS for debugging
		if (m_show_fps)
			std::cout << "Delta Time: " << delta_time << " | FPS: " << 1 / delta_time << std::endl;

		for (layer* layer : m_layer_stack)
			layer->on_update(delta_time);

		m_window->on_update();
		on_update_application();
	}
}

void application::on_event(event& e)
{
	for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
	{
		if (e.Handled())
			break;
		(*--it)->on_event(e);
	}
}

void application::on_window_close(window_closed& e)
{
	m_running = false;
}