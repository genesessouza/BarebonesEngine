#pragma once

#include "event.h"

class key_event : public event
{
public:
	inline key_event(unsigned int key) : keycode(key), keyname(get_key_name(key)) {}

	inline void invoke() override {}

	inline const std::string& get_name() const { return keyname; }
	inline unsigned int get_code() const { return keycode; }
protected:
	unsigned int keycode;
	std::string keyname;
};

class key_pressed : public key_event
{
public:
	inline key_pressed(unsigned int key) : key_event(key) {}
	inline ~key_pressed() {}

	inline void invoke() override { std::cout << "Key: [" << keyname << "] pressed." << std::endl; }
};

class key_released : public key_event
{
public:
	inline key_released(unsigned int key) : key_event(key) {}
	inline ~key_released() {}

	inline void invoke() override { std::cout << "Key: [" << keyname << "] released." << std::endl; }
};

class key_held : public key_event
{
public:
	inline key_held(unsigned int key) : key_event(key) {}
	inline ~key_held() {}

	inline void invoke() override { std::cout << "Key: [" << keyname << "] held..." << std::endl; }
};