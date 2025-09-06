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
	window_resized() : window_event() {}
};