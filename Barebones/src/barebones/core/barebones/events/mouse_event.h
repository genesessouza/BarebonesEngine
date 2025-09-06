#pragma once

#include "event.h"

class mouse_event : public event
{
public:
	inline mouse_event(int button) : mouse_button(button), button_name(get_mouse_button_name(button)) {}

	inline void invoke() override {}

	inline const std::string& get_name() const { return button_name; }
	inline unsigned int get_code() const { return mouse_button; }
protected:
	unsigned int mouse_button;
	std::string button_name;
};

class mouse_clicked : public mouse_event
{
public:
	inline mouse_clicked(unsigned int button) : mouse_event(button) {}

	inline void invoke() override { std::cout << "Mouse Button: [" << button_name << "] clicked." << std::endl; }
};

class mouse_released : public mouse_event
{
public:
	inline mouse_released(unsigned int button) : mouse_event(button) {}

	inline void invoke() override { std::cout << "Mouse Button: [" << button_name << "] released." << std::endl; }
};