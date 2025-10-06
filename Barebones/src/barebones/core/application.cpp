#include "application.h"
#include "barebones/events/event_queue.h"

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

	m_window = std::unique_ptr<glfw_window>(glfw_window::create());
	m_window->set_vsync(true);

	m_window->set_event_callback([this](event& e) { this->on_event(e); });
	m_event_layer = new event_layer();
	push_layer(m_event_layer);

	m_input_layer = new input_layer();
	push_overlay(m_input_layer);
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

		static float fps_timer = 0.0f;
		static int frame_count = 0;

		fps_timer += delta_time;
		frame_count++;

		if (fps_timer >= 0.1f) // Updates at 0.1 second intervals
		{
			int fps = frame_count;
			m_window->get_data().title = "Barebones Engine - FPS: " + std::to_string(fps * 10); // Multiply by 10 to get frame rate per second
			fps_timer = 0.0f;
			frame_count = 0;
		}

		for (layer* layer : m_layer_stack)
			layer->on_update(delta_time);

		m_window->on_update();
		on_update_application();

		event_queue::instance().run();
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