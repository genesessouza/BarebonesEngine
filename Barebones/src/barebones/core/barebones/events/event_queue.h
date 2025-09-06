#pragma once

#include "event.h"

#include <queue>
#include <iostream>

class event_queue
{
public:
	static event_queue& instance()
	{
		static event_queue instance;
		return instance;
	}

	void push(std::unique_ptr<event> ev)
	{
		queue.push(std::move(ev));
	}

	void run()
	{
		while (!queue.empty())
		{
			auto& ev = *queue.front();

			event_dispatcher::instance().dispatch(ev);

			queue.pop();
		}
	}

private:
	event_queue() = default;
	~event_queue() = default;

	event_queue(const event_queue&) = delete;
	event_queue& operator=(const event_queue&) = delete;

	std::queue<std::unique_ptr<event>> queue;
};