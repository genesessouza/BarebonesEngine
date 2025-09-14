#include "rendering_layer.h"

rendering_layer::rendering_layer(perspective_camera& camera)
	: main_light(nullptr), show_objects_on_scene_gizmo(true), show_objects_on_scene_bounds(true)
{
	scene_camera = &camera;
	scene_renderer.reset(new renderer(skybox_color));
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera);

	for (auto* obj : objects_on_scene)
	{
		scene_renderer->submit(obj, scene_camera, glm::mat4(1.0f));
		main_light->shine_on_objects(obj);

		if (show_objects_on_scene_gizmo)
		{
			obj->expose_gizmo()->set_position(obj->get_position());
			obj->expose_gizmo()->set_rotation(obj->get_rotation());

			obj->expose_gizmo()->render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
		}

		if (show_objects_on_scene_bounds)
		{
			obj->expose_bounds()->set_position(obj->get_position());
			obj->expose_bounds()->set_rotation(obj->get_rotation());
			obj->expose_bounds()->set_scale(obj->get_scale());

			obj->expose_bounds()->render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
		}
	}
}