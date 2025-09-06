#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class shader_data_type
{
	None = 0,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

static uint32_t shader_data_type_size(shader_data_type type)
{
	switch (type)
	{
	case shader_data_type::Float:   return 4;
	case shader_data_type::Float2:  return 4 * 2;
	case shader_data_type::Float3:  return 4 * 3;
	case shader_data_type::Float4:  return 4 * 4;
	case shader_data_type::Mat3:    return 4 * 3 * 3;
	case shader_data_type::Mat4:    return 4 * 4 * 4;
	case shader_data_type::Int:     return 4;
	case shader_data_type::Int2:    return 4 * 2;
	case shader_data_type::Int3:    return 4 * 3;
	case shader_data_type::Int4:    return 4 * 4;
	case shader_data_type::Bool:    return 1;
	}
	return 0;
}

struct buffer_element
{
	std::string name;
	shader_data_type type;
	uint32_t size;
	size_t offset;
	bool normalized;

	buffer_element() = default;

	buffer_element(shader_data_type type, const std::string& name, bool normalized = false)
		: name(name), type(type), size(shader_data_type_size(type)), offset(0), normalized(normalized)
	{
	}

	uint32_t get_component_count() const
	{
		switch (type)
		{
		case shader_data_type::Float:   return 1;
		case shader_data_type::Float2:  return 2;
		case shader_data_type::Float3:  return 3;
		case shader_data_type::Float4:  return 4;
		case shader_data_type::Mat3:    return 3 * 3;
		case shader_data_type::Mat4:    return 4 * 4;
		case shader_data_type::Int:     return 1;
		case shader_data_type::Int2:    return 2;
		case shader_data_type::Int3:    return 3;
		case shader_data_type::Int4:    return 4;
		case shader_data_type::Bool:    return 1;
		}
		return 0;
	}
};

class buffer_layout
{
public:
	buffer_layout() {}

	buffer_layout(const std::initializer_list<buffer_element>& elements)
		: elements(elements)
	{
		calculate_offsets_and_stride();
	}

	inline const std::vector<buffer_element>& get_elements() const { return elements; }
	inline uint32_t get_stride() const { return m_Stride; }

	std::vector<buffer_element>::iterator begin() { return elements.begin(); }
	std::vector<buffer_element>::iterator end() { return elements.end(); }

	std::vector<buffer_element>::const_iterator begin() const { return elements.begin(); }
	std::vector<buffer_element>::const_iterator end() const { return elements.end(); }

private:
	void calculate_offsets_and_stride()
	{
		size_t offset = 0;
		m_Stride = 0;
		for (auto& element : elements)
		{
			element.offset = offset;
			offset += element.size;
			m_Stride += element.size;
		}
	}

private:
	std::vector<buffer_element> elements;
	uint32_t m_Stride = 0;
};