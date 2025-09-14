/*
#include "playground.h"

#include "barebones/core/entry.h"

void playground::hard_code_single_triangle()
{
	float triangle[] = {
		-1.0f, -1.0f,
		 3.0f, -1.0f,
		-1.0f,  3.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

application* create_application()
{
	return new playground();
}

playground::playground()
{
	hard_code_single_triangle();

	sdf_shader->define_vec3("u_Resolution", glm::vec3(800, 600, 0));

	m_show_fps = true;
}

playground::~playground()
{

}

void playground::on_update_application()
{
	//glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float angleX = 0;
	static float angleY = 0;

	if (input_system()->get_key(GLFW_KEY_A))
		angleY -= 180 * delta_time;
	else if (input_system()->get_key(GLFW_KEY_D))
		angleY += 180 * delta_time;

	if (input_system()->get_key(GLFW_KEY_W))
		angleX += 180 * delta_time;
	else if (input_system()->get_key(GLFW_KEY_S))
		angleX -= 180 * delta_time;

	sdf_shader->define_vec3("u_RotationAngle", glm::vec3(glm::radians(angleX), glm::radians(angleY), 0));
	sdf_shader->define_float("u_Time", (float)glfwGetTime());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
*/