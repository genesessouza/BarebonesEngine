#include "event_layer.h"

#include "barebones/core/barebones/events/window_event.h"
#include "barebones/core/barebones/events/event_queue.h"

#include "barebones/core/application.h"

event_layer::event_layer()
{
	event_dispatcher::instance().subscribe<window_closed>([](window_closed& ev)
		{
			application::get().on_window_close(ev);
		});
}

void event_layer::on_update(timestep delta_time)
{
	event_queue::instance().run();
}