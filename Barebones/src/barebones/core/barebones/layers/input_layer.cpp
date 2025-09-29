#include "input_layer.h"

#include "barebones/core/barebones/events/event_queue.h"
#include "barebones/core/barebones/events/mouse_event.h"
#include "barebones/core/barebones/events/key_event.h"

std::unordered_map<int, bool> input_layer::previous_key_state;
std::unordered_map<int, bool> input_layer::current_key_state;
std::unordered_map<int, bool> input_layer::bttn_state;

std::pair<double, double> input_layer::last_mouse_position = { 0.0, 0.0 };

input_layer::input_layer()
{
	previous_key_state = current_key_state;

	event_dispatcher::instance().subscribe<key_pressed>([this](key_pressed& ev)
		{
			current_key_state[ev.get_code()] = true;
		});

	event_dispatcher::instance().subscribe<key_released>([this](key_released& ev)
		{
			current_key_state[ev.get_code()] = false;
		});

	event_dispatcher::instance().subscribe<mouse_clicked>([this](mouse_clicked& ev)
		{
			bttn_state[ev.get_code()] = true;
		});

	event_dispatcher::instance().subscribe<mouse_released>([this](mouse_released& ev)
		{
			bttn_state[ev.get_code()] = false;
		});

	event_dispatcher::instance().subscribe<mouse_moved>([this](mouse_moved& ev)
		{
			last_mouse_position.first = ev.get_x();
			last_mouse_position.second = ev.get_y();
		});
}

input_layer::~input_layer()
{

}

void input_layer::on_update(timestep delta_time)
{

}