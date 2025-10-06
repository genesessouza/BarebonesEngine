#include "physics_sample.h"

application* create_application()
{
	return new physics_sample();
}

static glm::vec3 blue_cube_initial_position = glm::vec3(1.5, 5, 1.5);
static glm::vec3 red_cube_initial_position = glm::vec3(0, 4.5, 0);

physics_sample::physics_sample()
{
	camera = new perspective_camera(45.0f, 800.0f, 600.0f, 0.1f, 50.0f);
	camera->set_position_and_rotation(glm::vec3(0, 3, 15), glm::vec3(0.0f));

	// FLOOR
	{
		floor.set_scale(glm::vec3(10, 0.3, 10));

		floor.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
		floor.set_static(true);
	}

	// MID AIR PLATFORM
	{
		mid_air_platform.set_position(glm::vec3(0, 3, 0), world);
		mid_air_platform.set_rotation(glm::vec3(30, 0, 0), local);
		mid_air_platform.set_scale(glm::vec3(5, 0.3, 5));

		mid_air_platform.get_material()->set_color(glm::vec4(0.7, 0.7, 0.7, 1));
		mid_air_platform.set_static(true);
	}

	// RED CUBE
	{
		red_cube.set_position(glm::vec3(0, 4.5, 0), world);

		red_cube.get_material()->set_color(glm::vec4(0.7, 0.3, 0.2, 1));
		red_cube.set_static(true);
	}

	// BLUE CUBE
	{
		blue_cube.set_position(glm::vec3(1.5, 5, 1.5), world);

		blue_cube.get_material()->set_color(glm::vec4(0.2, 0.3, 0.7, 1));
		blue_cube.set_static(true);
	}

	glm::vec4 sun = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);

	light_source = new light(light_type::directional, sun, 1);
	light_source->set_use_soft_shadows(false);
	light_source->set_position(glm::vec3(0, 999, -999), world);
	light_source->set_rotation(glm::vec3(60, 180, 0), local);

	// RENDERING LAYER
	{
		m_rendering_layer = new rendering_layer(*camera);

		m_rendering_layer->add_object(floor, false);
		m_rendering_layer->add_object(mid_air_platform, false);
		m_rendering_layer->add_object(red_cube, false);
		m_rendering_layer->add_object(blue_cube, false);

		m_rendering_layer->add_object(*light_source, true);

		push_overlay(m_rendering_layer);
	}

	// PHYSICS LAYER
	{
		m_physics_layer = new physics_layer();

		m_physics_layer->add_object(floor);
		m_physics_layer->add_object(mid_air_platform);
		m_physics_layer->add_object(red_cube);
		m_physics_layer->add_object(blue_cube);

		push_overlay(m_physics_layer);
	}

	get_window().set_vsync(false);
	m_show_fps = true;
}

static bool mouse_rotating = false;

void physics_sample::on_update_application()
{
	float sensitivity = 10.0f;
	glm::vec3 rotation_vector;

	// CAMERA ORBIT WITH MOUSE CONTROLS
	{
		auto& window = get_window();
		GLFWwindow* native_window = window.get_native_window();

		double center_x = window.get_width() / 2.0;
		double center_y = window.get_height() / 2.0;

		if (input_layer::get_button(GLFW_MOUSE_BUTTON_LEFT))
		{
			auto mouse_pos = input_layer::get_mouse_position();

			mouse_delta_x = mouse_pos.first - center_x;
			mouse_delta_y = mouse_pos.second - center_y;

			mouse_rotating = true;
		}
		else if (input_layer::get_button_up(GLFW_MOUSE_BUTTON_LEFT))
		{
			mouse_delta_x = 0.0f;
			mouse_delta_y = 0.0f;

			mouse_rotating = false;
		}

		rotation_vector = glm::vec3(-mouse_delta_y * sensitivity, -mouse_delta_x * sensitivity, 0);
	}

	// CAMERA KEYBOARD MOVEMENT CONTROLS
	{
		if (input_layer::get_key(GLFW_KEY_W))
			movement_vector.z -= 30 * delta_time;
		else if (input_layer::get_key(GLFW_KEY_S))
			movement_vector.z += 30 * delta_time;

		if (input_layer::get_key(GLFW_KEY_SPACE))
			movement_vector.y += 30 * delta_time;
		else if (input_layer::get_key(GLFW_KEY_LEFT_SHIFT))
			movement_vector.y -= 30 * delta_time;

		if (input_layer::get_key(GLFW_KEY_A))
			movement_vector.x -= 30 * delta_time;
		else if (input_layer::get_key(GLFW_KEY_D))
			movement_vector.x += 30 * delta_time;
	}

	camera->set_position_and_rotation(movement_vector, rotation_vector);

	// TOGGLE CUBES STATIC/PHYSICS
	{
		if (input_layer::get_key_down(GLFW_KEY_G))
		{
			red_cube.set_static(!red_cube.is_static_entity());
			blue_cube.set_static(!blue_cube.is_static_entity());
		}
	}

	if (red_cube.is_static_entity())
		red_cube.set_position(red_cube_initial_position, world);

	if (blue_cube.is_static_entity())
		blue_cube.set_position(blue_cube_initial_position, world);
}