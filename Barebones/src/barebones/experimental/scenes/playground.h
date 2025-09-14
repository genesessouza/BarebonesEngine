/*
#pragma once

#include "barebones/core/application.h"
#include "barebones/rendering/shader/shader.h"

class playground : public application
{
public:
	playground();
	~playground();

	void on_update_application() override;

	void hard_code_single_triangle();
private:
	unsigned int vao = 0, vbo = 0;

	std::shared_ptr<shader> sdf_shader = shader::instantiate(EXPERIMENTAL_SDF_SOURCE);
};
*/