#include "application.h"
#include "barebones/events/event_queue.h"

#include <format>

application* application::s_instance = nullptr;

application::application()
	: m_event_layer(nullptr), m_input_layer(nullptr)
{
	if (s_instance)
	{
		std::cout << "Application already running!" << std::endl;
		return;
	}

	s_instance = this;

	m_window = glfw_window::create();
	m_window->set_vsync(true);

	m_window->set_event_callback([this](event& e) { this->on_event(e); });

	auto evt = event_layer::create();
	m_event_layer = evt.get();
	push_layer(std::move(evt));

	auto inp = input_layer::create();
	m_input_layer = inp.get();
	push_overlay(std::move(inp));
}

application::~application()
{
	if (m_window)
	{
		glfwSetWindowUserPointer(m_window->get_native_window(), nullptr);
		glfwDestroyWindow(m_window->get_native_window());
		glfwMakeContextCurrent(nullptr);

		m_window.reset();
	}
}

void application::push_layer(std::unique_ptr<layer> layer)
{
	layer->on_attach();
	m_layer_stack.push_layer(std::move(layer));
}

void application::push_overlay(std::unique_ptr<layer> overlay)
{
	overlay->on_attach();
	m_layer_stack.push_overlay(std::move(overlay));
}

void application::run()
{
	float last_time = (float)glfwGetTime();
	float seconds_timer = 0.0f;
	int fps = 0;

	while (m_running)
	{
		float current_time = (float)glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;

		seconds_timer += delta_time;

		if (seconds_timer >= 1.0f) // Updates at 0.1 second intervals
		{
			m_window->get_data().title = std::format("Barebones Engine - FPS: {}", fps); // Multiply by 10 to get frame rate per second

			seconds_timer = 0.0f;
			fps = 0;
		}

		for (auto& layer : m_layer_stack)
			layer->on_update(delta_time);

		m_window->on_update();
		on_update_application();

		event_queue::instance().run();
		fps++;
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
	if (glfwWindowShouldClose(get_window().get_native_window()))
		m_running = false;
}