#include "rendering_layer.h"

rendering_layer::rendering_layer(perspective_camera& camera)
	: main_light(nullptr)
{
	scene_camera = &camera;
	scene_renderer.reset(new renderer(skybox_color));
}

void rendering_layer::add_object(entity& object_to_render, bool is_main_light)
{
	if (is_main_light)
		main_light = static_cast<light*>(&object_to_render);

	objects_on_scene.push_back(&object_to_render);
}

void rendering_layer::on_update(timestep deltatime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera);

	for (auto& obj : objects_on_scene)
	{
		scene_renderer->submit(obj, scene_camera, glm::mat4(1.0f));
		main_light->shine_on_objects(obj);
	}
}