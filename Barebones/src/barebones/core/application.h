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

	void push_layer(std::unique_ptr<layer> layer);
	void push_overlay(std::unique_ptr<layer> overlay);

	inline static application& get() { return *s_instance; }
	inline glfw_window& get_window() { return *m_window; }

	event_layer& get_event_layer() { return *m_event_layer; }

	void on_window_close(window_closed& e);
protected:
	virtual void on_update_application() = 0;
protected:
	bool m_running = true;

	timestep delta_time;
private:
	static application* s_instance;
	std::unique_ptr<glfw_window> m_window;

	event_layer* m_event_layer;
	input_layer* m_input_layer;

	layer_stack m_layer_stack;
};

application* create_application();