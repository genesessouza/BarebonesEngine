#include "sdf_raster_test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

application* create_application()
{
	return new sdf_raster_test();
}

sdf_raster_test::sdf_raster_test()
{
	// -------------------------------- SCENE ENTITIES ---------------------------
	{
		camera = new perspective_camera(45.0f, 800.0f, 600.0f, 0.1f, 100.0f);
		camera->set_position(glm::vec3(0, 1, 25));
		camera->set_rotation(glm::vec3(-90, 0, 0));

		floor.set_position(glm::vec3(0, 0, 0));
		floor.set_scale(glm::vec3(10, 0.3, 10));
		floor.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));

		back_wall.set_position(glm::vec3(0, 2.35, -5.15)); // behind floor end
		back_wall.set_scale(glm::vec3(10, 5, 0.3));
		back_wall.get_material()->set_color(glm::vec4(0.2, 0.2, 0.2, 1));

		cube1.set_position(glm::vec3(0, 3, 0));
		cube1.get_material()->set_color(glm::vec4(0.7, 0.3, 0.2, 1));

		glm::vec4 sun = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);

		light_source = new light(light_type::directional, sun, 1);
		light_source->set_position(glm::vec3(-3, 5, -5));
		light_source->set_rotation(glm::vec3(180, -15.5, 180));
	}

	// ---------------------------------- RENDERING & DEBUG -----------------------------
	{
		m_rendering_layer = new rendering_layer(*camera);

		m_rendering_layer->set_show_objects_on_scene_gizmo(true);
		m_rendering_layer->set_show_objects_on_scene_bounds(true);

		m_rendering_layer->add_object(floor, false);
		m_rendering_layer->add_object(back_wall, false);
		m_rendering_layer->add_object(cube1, false);

		m_rendering_layer->add_object(*light_source, true);

		push_overlay(m_rendering_layer);

		get_window().set_vsync(false);
		//m_show_fps = true;
	}
}

void sdf_raster_test::on_update_application()
{
	static glm::vec3 pos = glm::vec3(0, 5, 0); // for camera
	static glm::vec3 eulerRotation = glm::vec3(0, 0, 0);

	// ROTATION INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_UP))
			eulerRotation.x += 180 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_DOWN))
			eulerRotation.x -= 180 * delta_time;

		if (input_system()->get_key(GLFW_KEY_LEFT))
			eulerRotation.y -= 180 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_RIGHT))
			eulerRotation.y += 180 * delta_time;
	}

	// MOVEMENT INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_W))
			pos.z -= 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_S))
			pos.z += 30 * delta_time;

		if (input_system()->get_key(GLFW_KEY_SPACE))
			pos.y += 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_LEFT_SHIFT))
			pos.y -= 30 * delta_time;

		if (input_system()->get_key(GLFW_KEY_A))
			pos.x -= 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_D))
			pos.x += 30 * delta_time;
	}

	if (input_system()->get_key_down(GLFW_KEY_F))
	{
		if (light_source->get_light_type() == light_type::directional)
			light_source->set_light_type(light_type::point);
		else
			light_source->set_light_type(light_type::directional);
	}

	//light_source->set_position(glm::vec3(pos));
	light_source->set_rotation(eulerRotation);

	//std::cout << light_source->get_rotation().x << ", " << light_source->get_rotation().y << ", " << light_source->get_rotation().z << std::endl;

	//camera->set_rotation(glm::vec3(eulerRotation.x, 0, 0));
	//camera->set_position(glm::vec3(pos.x, pos.y, pos.z));

	//std::cout << camera->get_rotation().x << ", " << camera->get_rotation().y << ", " << camera->get_rotation().z << std::endl;
	//std::cout << camera->get_position().x << ", " << camera->get_position().y << ", " << camera->get_position().z << std::endl;
}