#include "rendering_layer.h"

rendering_layer::rendering_layer(perspective_camera& camera)
	: main_light(nullptr)
{
	scene_camera = &camera;
	scene_renderer.reset(new renderer(skybox_color));

	m_fbo = new frame_buffer(4096, 4096); // hard coded resolution for now
}

void rendering_layer::add_object(entity& object_to_render, bool is_main_light)
{
	if (is_main_light)
		main_light = static_cast<light*>(&object_to_render);

	objects_on_scene.push_back(&object_to_render);
}

void rendering_layer::on_update(timestep delta_time)
{
	draw_depth_pass();

	draw_render_pass();
}

void rendering_layer::draw_depth_pass()
{
	m_fbo->render_depth_map(main_light, scene_camera);
	
	scene_renderer->begin_scene(scene_camera, main_light);

	for (auto* obj : objects_on_scene)
	{
		m_fbo->get_depth_shader()->define_mat4("u_model", &obj->get_model_matrix()[0][0]);
		scene_renderer->submit_depth(obj, m_fbo->get_light_view(), m_fbo->get_light_proj(), m_fbo->get_light_space());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 800, 600); // set resolution back to window width/height
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_fbo->draw_screen_quad();
}

void rendering_layer::draw_render_pass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera, main_light);

	for (auto* obj : objects_on_scene)
	{
		scene_renderer->submit(obj, m_fbo->get_light_space(), m_fbo);
		main_light->shine_on_objects(obj, scene_camera);
	}
}