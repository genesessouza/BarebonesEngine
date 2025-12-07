#pragma once

#include "layer.h"
#include <vector>
#include <memory>
#include <algorithm>

class layer_stack
{
public:
	layer_stack();
	~layer_stack();

	void push_layer(std::unique_ptr<layer>&& layer);
	void pop_layer(layer* layer);

	void push_overlay(std::unique_ptr<layer>&& overlay);
	void pop_overlay(layer* overlay);

	inline auto begin() { return m_layers.begin(); }
	inline auto end() { return m_layers.end(); }

	void clear_stack();
private:
	std::vector<std::unique_ptr<layer>> m_layers;
	unsigned int m_layer_insert_index = 0;
};