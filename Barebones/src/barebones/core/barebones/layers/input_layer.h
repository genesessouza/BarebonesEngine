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

	static bool get_key_down(int keycode) { return current_key_state[keycode] && !previous_key_state[keycode]; }
	static bool get_key(int keycode) { return current_key_state[keycode]; }
	static bool get_key_up(int keycode) { return !current_key_state[keycode] && previous_key_state[keycode]; }

	static bool get_button_down(int bttn) { return current_bttn_state[bttn] && !previous_bttn_state[bttn]; }
	static bool get_button(int bttn) { return current_bttn_state[bttn]; }
	static bool get_button_up(int bttn) { return !current_bttn_state[bttn] && previous_bttn_state[bttn]; }

	static std::pair<double, double> get_mouse_position() { return last_mouse_position; }
private:
	static std::unordered_map<int, bool> previous_key_state;
	static std::unordered_map<int, bool> current_key_state;
	static std::unordered_map<int, bool> previous_bttn_state;
	static std::unordered_map<int, bool> current_bttn_state;

	static std::pair<double, double> last_mouse_position;
};
