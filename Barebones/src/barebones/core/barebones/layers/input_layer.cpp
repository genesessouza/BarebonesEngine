#include "input_layer.h"

#include "../events/event_queue.h"
#include "../events/mouse_event.h"
#include "../events/key_event.h"

input_layer::input_layer()
{
	event_dispatcher::instance().subscribe<key_pressed>([this](key_pressed& ev)
		{
			key_state[ev.get_code()] = true;
		});

	event_dispatcher::instance().subscribe<key_released>([this](key_released& ev)
		{
			key_state[ev.get_code()] = false;
		});

	event_dispatcher::instance().subscribe<mouse_clicked>([this](mouse_clicked& ev)
		{
			bttn_state[ev.get_code()] = true;
		});

	event_dispatcher::instance().subscribe<mouse_released>([this](mouse_released& ev)
		{
			bttn_state[ev.get_code()] = false;
		});
}

void input_layer::on_update(timestep delta_time)
{
	event_queue::instance().run();
}