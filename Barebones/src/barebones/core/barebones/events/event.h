#pragma once

#include "keymap.h"

#include <unordered_map>
#include <typeindex>
#include <functional>

class event
{
public:
	friend class event_dispatcher;

	virtual ~event() = default;
	virtual void invoke() = 0;

	const bool Handled() const { return m_handled; }
private:
	bool m_handled = false;
};

class event_dispatcher
{
public:
	static event_dispatcher& instance()
	{
		static event_dispatcher dispatcher;
		return dispatcher;
	}

	template<typename EventType>
	void subscribe(std::function<void(EventType&)> callback)
	{
		auto wrapper = [cb = std::move(callback)](event& ev) { cb(static_cast<EventType&>(ev)); };
		listeners[typeid(EventType)].push_back(wrapper);
	}

	void dispatch(event& ev)
	{
		auto it = listeners.find(typeid(ev));
		if (it != listeners.end())
		{
			for (auto& listener : it->second)
				listener(ev);
		}
	}
private:
	event_dispatcher() = default;
	std::unordered_map<std::type_index, std::vector<std::function<void(event&)>>> listeners;
};