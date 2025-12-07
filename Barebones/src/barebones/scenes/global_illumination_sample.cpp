#include "global_illumination_sample.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//application* create_application()
//{
//	return new global_illumination_sample();
//}

global_illumination_sample::global_illumination_sample()
{
	// -------------------------------- SCENE ENTITIES ---------------------------
	{
		// CAMERA
		{
			camera = new perspective_camera(45.0f, 800.0f, 600.0f, 0.1f, 50.0f);
			camera->set_position_and_rotation(glm::vec3(0, 3, 25), glm::vec3(0.0f));
		}

		// FLOOR
		{
			floor.set_scale(glm::vec3(10, 0.3, 10));

			floor.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			floor.set_static(true);
		}

		// FRONT WALL
		{
			front_wall.set_position(glm::vec3(0, 1.15, 5.15), world); // behind floor end
			front_wall.set_scale(glm::vec3(10, 3, 0.3));

			front_wall.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			front_wall.set_static(true);
		}

		// BACK WALL
		{
			back_wall.set_position(glm::vec3(0, 2.35, -5.15), world); // behind floor end
			back_wall.set_scale(glm::vec3(10, 5, 0.3));

			back_wall.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			back_wall.set_static(true);
		}

		// LEFT WALL
		{
			left_wall.set_position(glm::vec3(-5, 2.5, 0), world); // behind floor end
			//left_wall.set_rotation(glm::vec3(0, 0, 0), local);
			left_wall.set_scale(glm::vec3(0.3, 5, 10));

			left_wall.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			left_wall.set_static(true);
		}

		// RIGHT WALL
		{
			right_wall.set_position(glm::vec3(5, 2.5, 0), world); // behind floor end
			//right_wall.set_rotation(glm::vec3(0, 0, 0), local);
			right_wall.set_scale(glm::vec3(0.3, 5, 10));

			right_wall.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			right_wall.set_static(true);
		}

		// CEILING
		{
			mid_air_platform.set_scale(glm::vec3(10, 0.3, 10));
			mid_air_platform.set_position(glm::vec3(0, 5, 0), world);

			mid_air_platform.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));
			mid_air_platform.set_static(true);
		}

		// RED CUBE
		{
			red_cube.set_position(glm::vec3(0, 3, 0), world);

			red_cube.get_material()->set_color(glm::vec4(0.7, 0.3, 0.2, 1));
		}

		// BLUE CUBE
		{
			blue_cube.set_position(glm::vec3(-3, 0.65, 3), world);

			blue_cube.get_material()->set_color(glm::vec4(0.2, 0.3, 0.7, 1));
		}

		// GREEN CUBE
		{
			green_cube.set_position(glm::vec3(3, 2, 1), world);
			//green_cube.set_rotation(glm::vec3(45, 0, 0), local);
			green_cube.set_scale(glm::vec3(2, 1, 3));

			green_cube.get_material()->set_color(glm::vec4(0.3, 0.7, 0.2, 1));
			green_cube.set_static(true);
		}

		glm::vec4 sun = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);

		light_source = new light(light_type::directional, sun, 1);
		light_source->set_use_soft_shadows(true);
		light_source->set_position(glm::vec3(0, 7, 0), world);
	}

	// ---------------------------------- RENDERING & DEBUG -----------------------------
	{
		auto rendering_layer = rendering_layer::create(*camera);
		m_rendering_layer = rendering_layer.get();

		m_rendering_layer->add_object(floor, false);
		m_rendering_layer->add_object(mid_air_platform, false);
		m_rendering_layer->add_object(front_wall, false);
		m_rendering_layer->add_object(back_wall, false);
		m_rendering_layer->add_object(left_wall, false);
		m_rendering_layer->add_object(right_wall, false);
		m_rendering_layer->add_object(red_cube, false);
		m_rendering_layer->add_object(blue_cube, false);
		m_rendering_layer->add_object(green_cube, false);

		m_rendering_layer->add_object(*light_source, true);

		push_overlay(std::move(rendering_layer));

		get_window().set_vsync(false);
	}
}

bool handle_input = true;
static bool mouse_rotating = false;

void global_illumination_sample::on_update_application()
{
	static glm::vec3 pos = glm::vec3(0, 3, 15); // for camera
	static glm::vec3 euler_rotation = glm::vec3(0, 0, 0);

	// INPUT HANDLING
	{
		if (input_layer::get_key_down(GLFW_KEY_F))
		{
			if (handle_input)
				handle_input = false;
			else
				handle_input = true;
		}

		if (input_layer::get_key_down(GLFW_KEY_C))
		{
			if (light_source->get_use_soft_shadows())
				light_source->set_use_soft_shadows(false);
			else
				light_source->set_use_soft_shadows(true);
		}

		if (input_layer::get_key_down(GLFW_KEY_L))
		{
			if (light_source->get_light_type() == light_type::directional)
				light_source->set_light_type(light_type::point);
			else
				light_source->set_light_type(light_type::directional);
		}

		if (handle_input)
		{
			// ROTATION INPUT //
			{
				if (input_layer::get_key(GLFW_KEY_UP))
					euler_rotation.x += 180 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_DOWN))
					euler_rotation.x -= 180 * delta_time;

				if (input_layer::get_key(GLFW_KEY_LEFT))
					euler_rotation.y -= 180 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_RIGHT))
					euler_rotation.y += 180 * delta_time;

				if (input_layer::get_key(GLFW_KEY_Q))
					euler_rotation.z -= 180 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_E))
					euler_rotation.z += 180 * delta_time;
			}

			// MOVEMENT INPUT //
			{
				if (input_layer::get_key(GLFW_KEY_W))
					pos.z -= 30 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_S))
					pos.z += 30 * delta_time;

				if (input_layer::get_key(GLFW_KEY_SPACE))
					pos.y += 30 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_LEFT_SHIFT))
					pos.y -= 30 * delta_time;

				if (input_layer::get_key(GLFW_KEY_A))
					pos.x -= 30 * delta_time;
				else if (input_layer::get_key(GLFW_KEY_D))
					pos.x += 30 * delta_time;
			}
		}
	}

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

	//light_source->set_rotation(euler_rotation, local);
	//light_source->set_position(pos, world);

	//std::cout << light_source->get_rotation().x << ", " << light_source->get_rotation().y << ", " << light_source->get_rotation().z << std::endl;

	camera->set_position_and_rotation(pos, rotation_vector);

	//std::cout << camera->get_rotation().x << ", " << camera->get_rotation().y << ", " << camera->get_rotation().z << std::endl;
	//std::cout << camera->get_position().x << ", " << camera->get_position().y << ", " << camera->get_position().z << std::endl;
}