#include "sdf_raster_test.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

application* create_application()
{
	return new sdf_raster_test();
}

sdf_raster_test::sdf_raster_test()
{
	// -------------------------------- SCENE ENTITIES ---------------------------
	{
		camera = new perspective_camera(45.0f, 800.0f / 600.0f, 0.1f, 100.0f, SDF_INTEGRATION_SOURCE);
		camera->set_position(glm::vec3(0, 3, 15));

		cube1.get_material()->get_shader()->define_bool("u_quad", false);
		cube1.set_position(glm::vec3(-2, 3, -2));

		quad1.get_material()->get_shader()->define_bool("u_quad", true);
		quad1.set_position(glm::vec3(0, 0, 0));
		quad1.set_rotation(glm::vec3(0, 0, 0));
		quad1.set_scale(glm::vec3(10.0f, 1.0f, 10.0f));

		quad1.get_material()->set_color(glm::vec4(1, 1, 1, 1)); // blue
		cube1.get_material()->set_color(glm::vec4(0.7, 0, 0, 1)); // red

		glm::vec4 sun = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);

		light_source = new light(light_type::directional, sun, 1);
		light_source->set_position(glm::vec3(0, 3, 0));
		light_source->set_rotation(glm::vec3(-70, 0, 0));
	}

	// ---------------------------------- RENDERING & DEBUG -----------------------------
	{
		m_rendering_layer = new rendering_layer(*camera);

		m_rendering_layer->add_object(quad1, false);
		m_rendering_layer->add_object(cube1, false);
		m_rendering_layer->add_object(*light_source, true);

		push_overlay(m_rendering_layer);

		get_window().set_vsync(false);
		m_show_fps = true;
	}
}

void sdf_raster_test::on_update_application()
{
	glDisable(GL_CULL_FACE);

	// ROTATION INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_UP))
			xRot += 180 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_DOWN))
			xRot -= 180 * delta_time;

		if (input_system()->get_key(GLFW_KEY_LEFT))
			yRot -= 180 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_RIGHT))
			yRot += 180 * delta_time;
	}

	// MOVEMENT INPUT //
	{
		if (input_system()->get_key(GLFW_KEY_W))
			zPos -= 18 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_S))
			zPos += 18 * delta_time;

		if (input_system()->get_key(GLFW_KEY_SPACE))
			yPos += 18 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_LEFT_SHIFT))
			yPos -= 18 * delta_time;

		if (input_system()->get_key(GLFW_KEY_A))
			xPos -= 18 * delta_time;
		else if (input_system()->get_key(GLFW_KEY_D))
			xPos += 18 * delta_time;
	}

	if (input_system()->get_key_down(GLFW_KEY_F))
	{
		if (light_source->get_light_type() == light_type::directional)
		{
			light_source->set_light_type(light_type::point);

			quad1.get_material()->get_shader()->define_bool("u_directionalLight", false);
			cube1.get_material()->get_shader()->define_bool("u_directionalLight", false);
		}
		else
		{
			light_source->set_light_type(light_type::directional);

			quad1.get_material()->get_shader()->define_bool("u_directionalLight", true);
			cube1.get_material()->get_shader()->define_bool("u_directionalLight", true);
		}
	}

	light_source->set_rotation(glm::vec3(xRot, yRot, 0));
	light_source->set_position(glm::vec3(xPos, yPos, zPos));

	//cube1.set_rotation(glm::vec3(xRot, yRot, 0));
	//cube1.set_position(glm::vec3(xPos, yPos, zPos));

	glm::vec3 cubeScl = cube1.get_scale();
	glm::vec3 cubePos = cube1.get_position();

	// --------------------------- GETS CUBE SCALE AND POS FOR RAY EXIT - DOESNT SUPPORT OBJECT ROTATION (YET!) ------------------------------ //

	quad1.get_material()->get_shader()->define_vec3("u_cubeMin", glm::vec3(-(cubeScl.x / 2) + (cubePos.x / 2), (cubeScl.y / 2) + (cubePos.y / 2), -(cubeScl.z / 2) + (cubePos.z / 2))); // -0.5, 0.5, -0.5
	quad1.get_material()->get_shader()->define_vec3("u_cubeMax", glm::vec3((cubeScl.x / 2) + (cubePos.x / 2), cubePos.y / 2, (cubeScl.z / 2) + (cubePos.z / 2)));

	// --------------------------------------------------------------------------------------------------------------------------------------- //

	quad1.get_material()->get_shader()->define_vec3("u_lightPos", light_source->get_position());
	cube1.get_material()->get_shader()->define_vec3("u_lightPos", light_source->get_position());
}