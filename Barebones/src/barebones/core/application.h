#pragma once

#include "window/glfw_window.h"

#include "barebones/timestep.h"

#include "barebones/events/window_event.h"

#include "barebones/layers/event_layer.h"
#include "barebones/layers/input_layer.h"

#include "layer.h"
#include "layer_stack.h"

class application
{
public:
	application();
	virtual ~application();

	virtual void run();
	virtual void on_event(event& e);

	void push_layer(layer* layer);
	void push_overlay(layer* overlay);

	inline static application& get() { return *s_instance; }
	inline glfw_window& get_window() { return *m_window; }

	inline event_layer* event_system() const { return m_event_layer; }
	inline input_layer* input_system() const { return m_input_layer; }

	void on_window_close(window_closed& e);
protected:
	virtual void on_update_application() = 0;
protected:
	bool m_running = true;
	float m_last_frame_time = 0.0f;
	bool m_show_fps = false;

	timestep delta_time;
private:
	event_layer* m_event_layer;
	input_layer* m_input_layer;
private:
	static application* s_instance;
	std::unique_ptr<glfw_window> m_window;
	layer_stack m_layer_stack;
};

application* create_application();