#include "rendering_layer.h"

rendering_layer::rendering_layer(perspective_camera& camera)
	: main_light(nullptr), show_objects_on_scene_gizmo(true), show_objects_on_scene_bounds(true)
{
	scene_camera = &camera;
	scene_renderer.reset(new renderer(skybox_color));

	m_fbo = new frame_buffer(800, 600); // hard coded resolution for now
}

void rendering_layer::add_object(entity& object_to_render, bool is_main_light)
{
	if (is_main_light)
		main_light = static_cast<light*>(&object_to_render);

	objects_on_scene.push_back(&object_to_render);

	objects_on_scene_gizmo.push_back(object_to_render.expose_gizmo());
	objects_on_scene_bounds.push_back(object_to_render.expose_bounds());
}

void rendering_layer::on_update(timestep delta_time)
{
	draw_depth_pass();

	draw_render_pass();
}

void rendering_layer::draw_depth_pass()
{
	scene_renderer->begin_scene(scene_camera, main_light);

	// CHECK ON RENDERER TO SEE HOW THOSE ARE SET
	{
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
	}

	glViewport(0, 0, m_fbo->get_width(), m_fbo->get_height());
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->get());
	glClear(GL_DEPTH_BUFFER_BIT);

	light_view_matrix = glm::lookAt(main_light->get_position(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	light_proj_matrix = m_shadow_map_ortho_matrix;

	m_fbo->get_depth_shader()->bind();
	m_fbo->get_depth_shader()->define_mat4("u_lightView", &light_view_matrix[0][0]);
	m_fbo->get_depth_shader()->define_mat4("u_lightProjection", &light_proj_matrix[0][0]);

	for (auto* obj : objects_on_scene)
	{
		m_fbo->get_depth_shader()->define_mat4("u_model", &obj->get_model_matrix()[0][0]);
		scene_renderer->submit_depth(obj, light_view_matrix, light_proj_matrix);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_fbo->get_width(), m_fbo->get_height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_screen_quad();
}

void rendering_layer::draw_render_pass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera, main_light);

	glm::mat4 light_space_matrix = light_proj_matrix * light_view_matrix;

	for (auto* obj : objects_on_scene)
	{
		scene_renderer->submit(obj, light_space_matrix, m_fbo);
		main_light->shine_on_objects(obj);

		glDisable(GL_DEPTH_TEST);
		if (show_objects_on_scene_gizmo)
		{
			obj->expose_gizmo()->set_position(obj->get_position());
			obj->expose_gizmo()->set_rotation(obj->get_rotation());

			obj->expose_gizmo()->render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
		}
		glEnable(GL_DEPTH_TEST);

		if (show_objects_on_scene_bounds)
		{
			obj->expose_bounds()->set_position(obj->get_position());
			obj->expose_bounds()->set_rotation(obj->get_rotation());
			obj->expose_bounds()->set_scale(obj->get_scale());

			obj->expose_bounds()->render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
		}
	}
}

void rendering_layer::draw_screen_quad()
{
	m_fbo->get_debug_shader()->bind();
	m_fbo->get_debug_shader()->define_int("depthMap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo->get_depth_texture());

	m_fbo->get_vao()->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}