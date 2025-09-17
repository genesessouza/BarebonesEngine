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
		camera->set_position(glm::vec3(0, 5, -20));
		camera->set_rotation(glm::vec3(0, 0, 0));

		floor.set_position(glm::vec3(0, 0, 0));
		floor.set_scale(glm::vec3(10, 0.3, 10));
		floor.get_material()->set_color(glm::vec4(0.4, 0.4, 0.4, 1)); // red

		back_wall.set_position(glm::vec3(0, 2.3, -5)); // behind floor end
		back_wall.set_scale(glm::vec3(10, 5, 0.3));
		back_wall.get_material()->set_color(glm::vec4(0.4, 0.4, 0.4, 1)); // red

		glm::vec4 sun = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);

		light_source = new light(light_type::directional, sun, 1);
		light_source->set_position(glm::vec3(5, 3, 5));
	}

	// ---------------------------------- RENDERING & DEBUG -----------------------------
	{
		m_rendering_layer = new rendering_layer(*camera);

		m_rendering_layer->set_show_objects_on_scene_gizmo(true);
		m_rendering_layer->set_show_objects_on_scene_bounds(false);

		m_rendering_layer->add_object(floor, false);
		m_rendering_layer->add_object(back_wall, false);

		m_rendering_layer->add_object(*light_source, true);

		push_overlay(m_rendering_layer);

		get_window().set_vsync(false);
		m_show_fps = true;
	}
}

void sdf_raster_test::on_update_application()
{
	// ROTATION INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_UP))
			xRot -= 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_DOWN))
			xRot += 30 * delta_time;

		if (input_system()->get_key(GLFW_KEY_LEFT))
			yRot += 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_RIGHT))
			yRot -= 30 * delta_time;
	}

	// MOVEMENT INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_W))
			zPos += 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_S))
			zPos -= 30 * delta_time;

		if (input_system()->get_key(GLFW_KEY_SPACE))
			yPos += 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_LEFT_SHIFT))
			yPos -= 30 * delta_time;

		if (input_system()->get_key(GLFW_KEY_A))
			xPos -= 30 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_D))
			xPos += 30 * delta_time;
	}

	if (input_system()->get_key_down(GLFW_KEY_F))
	{
		if (light_source->get_light_type() == light_type::directional)
			light_source->set_light_type(light_type::point);
		else
			light_source->set_light_type(light_type::directional);
	}

	camera->set_position(camera->local_right() * -xPos + camera->local_up() * -yPos + camera->local_forward() * -zPos);
	camera->set_rotation(camera->local_right() * -xRot + camera->local_up() * -yRot);
}