#pragma once

#include "barebones/core/barebones/events/event.h"
#include "barebones/core/barebones/timestep.h"
#include "barebones/core/layer.h"

class input_layer : public layer
{
public:
	input_layer();
	~input_layer();

	void on_update(timestep delta_time);

	bool get_key(int keycode) { return current_key_state[keycode]; }
	bool get_key_down(int keycode) { return current_key_state[keycode] && !previous_key_state[keycode]; }

	bool get_button(int bttn) { return bttn_state[bttn]; }
private:
	std::unordered_map<int, bool> previous_key_state;
	std::unordered_map<int, bool> current_key_state;
	std::unordered_map<int, bool> bttn_state;
};
