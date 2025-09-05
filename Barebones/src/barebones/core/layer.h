#pragma once

#include "barebones/timestep.h"
#include "barebones/events/event.h"

#include <string>

class layer
{
public:
	layer(const std::string& name = "Layer");
	virtual ~layer();

	virtual void on_attach();
	virtual void on_detach();
	virtual void on_update(timestep timestep);
	virtual void on_event(event& event);

	inline const std::string& get_name() const { return m_debug_name; }
protected:
	std::string m_debug_name;
};
