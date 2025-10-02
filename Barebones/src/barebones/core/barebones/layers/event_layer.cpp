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
	
	event_dispatcher::instance().subscribe<window_resized>([](window_resized& ev)
		{
			auto& data = application::get().get_window().get_data();
			data.width = ev.get_width();
			data.height = ev.get_height();

			glViewport(0, 0, data.width, data.height);
		});
}

void event_layer::update_aspect_ratio(perspective_camera* camera, int width, int height)
{
	camera->set_projection(camera->get_fov(), (float) width / (float) height, camera->get_near_clip(), camera->get_far_clip());
}

void event_layer::on_update(timestep delta_time)
{
}