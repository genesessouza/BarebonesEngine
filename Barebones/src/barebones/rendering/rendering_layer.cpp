#include "rendering_layer.h"

rendering_layer::rendering_layer(perspective_camera& camera)
	: main_light(nullptr), show_object_gizmo(true), show_object_bounds(true)
{
	scene_camera = &camera;
	scene_renderer.reset(new renderer(skybox_color));
}

void rendering_layer::add_object(entity& object_to_render, bool is_main_light)
{
	if (is_main_light)
		main_light = static_cast<light*>(&object_to_render);

	objects_on_scene.push_back(&object_to_render);

	for (auto* object_on_scene : objects_on_scene)
	{
		if (show_object_gizmo)
		{
			gizmo g = *object_on_scene->expose_gizmo();

			g.set_position(object_on_scene->get_position());
			g.set_rotation(object_on_scene->get_rotation());

			objects_gizmo_on_scene.push_back(g);
		}

		if (show_object_bounds)
		{
			bounds b = *object_on_scene->expose_bounds();
			b.set_position(object_on_scene->get_position());
			b.set_rotation(object_on_scene->get_rotation());

			objects_bounds_on_scene.push_back(b);
		}
	}
}

void rendering_layer::on_update(timestep deltatime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera);

	for (auto* object_on_scene : objects_on_scene)
	{
		scene_renderer->submit(object_on_scene, scene_camera, glm::mat4(1.0f));
		main_light->shine_on_objects(object_on_scene);

		if (show_object_gizmo)
		{
			for (auto& object_on_scene_gizmo : objects_gizmo_on_scene)
			{
				object_on_scene_gizmo.set_position(object_on_scene->get_position());
				object_on_scene_gizmo.set_rotation(object_on_scene->get_rotation());

				object_on_scene_gizmo.render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
			}
		}

		if (show_object_bounds)
		{
			for (auto& object_on_scene_bounds : objects_bounds_on_scene)
			{
				object_on_scene_bounds.set_position(object_on_scene->get_position());
				object_on_scene_bounds.set_rotation(object_on_scene->get_rotation());

				object_on_scene_bounds.render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());
			}
		}
	}
}