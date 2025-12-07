#include "rendering_layer.h"

std::unique_ptr<rendering_layer> rendering_layer::create(perspective_camera& camera)
{
	return std::make_unique<rendering_layer>(camera);
}

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

	m_fbo->draw_screen_quad();
}

void rendering_layer::draw_render_pass()
{
	update_aspect_ratio();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera, main_light);

	for (auto* obj : objects_on_scene)
	{
		scene_renderer->submit(obj, m_fbo->get_light_space(), m_fbo);
		obj->update_debug(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
		main_light->shine_on_objects(obj, scene_camera);
	}
}

void rendering_layer::update_aspect_ratio()
{
	float initial_width = app.get_window().get_width();
	float initial_height = app.get_window().get_height();

	float app_aspect = initial_width / initial_height;

	int width;
	int height;

	glfwGetWindowSize(app.get_window().get_native_window(), &width, &height);

	float window_aspect = (float)width / (float)height;

	if (window_aspect > app_aspect)
	{
		int new_width = (int)(width * app_aspect / window_aspect + 0.5f);
		int bar_width = (width - new_width) / 2;
		glViewport(bar_width, 0, new_width, height); // set resolution to maintain aspect ratio
	}
	else
	{
		int new_height = (int)(height * window_aspect / app_aspect + 0.5f);
		int bar_height = (height - new_height) / 2;

		glViewport(0, bar_height, width, new_height); // set resolution to maintain aspect ratio
	}

	scene_camera->set_projection(scene_camera->get_fov(), app_aspect, scene_camera->get_near_clip(), scene_camera->get_far_clip());
}