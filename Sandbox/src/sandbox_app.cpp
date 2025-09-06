#include "sandbox_app.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

application* create_application()
{
	return new app();
}

app::app()
{
	// -------------------------------- SCENE ENTITIES ---------------------------
	{
		camera = new perspective_camera(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		camera->set_position(glm::vec3(0, 3, 15));

		cube1.set_position(glm::vec3(-2, 1, 0));

		quad1.set_scale(glm::vec3(10.0f, 1.0f, 10.0f));

		quad1.get_material()->set_color(glm::vec4(0, 0, 0.7, 1)); // blue
		cube1.get_material()->set_color(glm::vec4(0.7, 0, 0, 1)); // red

		light_source = new light(light_type::directional, sun, 1);
		light_source->set_position(glm::vec3(3, 5, 0));
		light_source->set_color(sun, 1);
	}

	// ---------------------------------- RENDERING & DEBUG -----------------------------
	{
		m_rendering_layer = new rendering_layer(*camera);
		m_rendering_layer->add_object(quad1, false);
		m_rendering_layer->add_object(cube1, false);
		m_rendering_layer->add_object(*light_source, true);
		push_overlay(m_rendering_layer);

		//m_show_fps = true;
	}
}

void app::on_update_application()
{
	if (input_system()->get_key(GLFW_KEY_ESCAPE))
		m_running = false;

	if (input_system()->get_key(GLFW_KEY_UP))
		xRot += 180 * delta_time;
	else if (input_system()->get_key(GLFW_KEY_DOWN))
		xRot -= 180 * delta_time;

	if (input_system()->get_key(GLFW_KEY_LEFT))
		yRot += 180 * delta_time;
	else if (input_system()->get_key(GLFW_KEY_RIGHT))
		yRot -= 180 * delta_time;

	if (input_system()->get_key_down(GLFW_KEY_F))
	{
		if (light_source->get_light_type() == light_type::directional)
			light_source->set_light_type(light_type::point);
		else
			light_source->set_light_type(light_type::directional);
	}

	if (input_system()->get_key_down(GLFW_KEY_G))
		m_rendering_layer->set_show_objects_on_scene_gizmo(!m_rendering_layer->get_show_objects_on_scene_gizmo());

	if (input_system()->get_key_down(GLFW_KEY_B))
		m_rendering_layer->set_show_objects_on_scene_bounds(!m_rendering_layer->get_show_objects_on_scene_bounds());

	//light_source->set_position(glm::vec3(xRot, yRot, zRot));

	light_source->set_rotation(glm::vec3(xRot, yRot, zRot));
	//cube1.set_rotation(glm::vec3(xRot, yRot, zRot));
	//quad1.set_rotation(glm::vec3(xRot, yRot, zRot));
}