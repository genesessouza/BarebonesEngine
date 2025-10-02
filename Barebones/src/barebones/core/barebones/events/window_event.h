#pragma once

#include "event.h"

class window_event : public event
{
public:
	void invoke() override {}
};

class window_closed : public window_event
{
public:
	window_closed() : window_event() {}
};

class window_resized : public window_event
{
public:
	window_resized(int width, int height) : window_event(), m_width(width), m_height(height) {}

	int get_width() const { return m_width; }
	int get_height() const { return m_height; }
private:
	int m_width;
	int m_height;
};