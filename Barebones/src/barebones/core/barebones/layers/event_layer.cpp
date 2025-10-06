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
	
	event_dispatcher::instance().subscribe<window_resized>([this](window_resized& ev)
		{
			auto& data = application::get().get_window().get_data();
			data.width = ev.get_width();
			data.height = ev.get_height();
		});
}

void event_layer::on_update(timestep delta_time)
{
}