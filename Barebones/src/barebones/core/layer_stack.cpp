#include "layer_stack.h"

layer_stack::layer_stack() = default;

layer_stack::~layer_stack()
{
	for (auto& layer : m_layers)
		layer->on_detach();
}

void layer_stack::push_layer(std::unique_ptr<layer>&& layer)
{
	m_layers.emplace(m_layers.begin() + m_layer_insert_index, std::move(layer));
	m_layer_insert_index++;
}

void layer_stack::push_overlay(std::unique_ptr<layer>&& overlay)
{
	m_layers.emplace_back(std::move(overlay));
}

void layer_stack::pop_layer(layer* l)
{
	auto it = std::find_if(m_layers.begin(), m_layers.begin() + m_layer_insert_index,
		[l](const std::unique_ptr<layer>& ptr) { return ptr.get() == l; });

	if (it != m_layers.begin() + m_layer_insert_index)
	{
		(*it)->on_detach();
		m_layers.erase(it);
		m_layer_insert_index--;
	}
}

void layer_stack::pop_overlay(layer* overlay)
{
	auto it = std::find_if(m_layers.begin() + m_layer_insert_index, m_layers.end(),
		[overlay](const std::unique_ptr<layer>& ptr) { return ptr.get() == overlay; });

	if (it != m_layers.end())
	{
		(*it)->on_detach();
		m_layers.erase(it);
	}
}

void layer_stack::clear_stack()
{
}