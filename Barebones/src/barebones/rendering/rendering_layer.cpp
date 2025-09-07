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

	objects_on_scene_gizmo.push_back(*object_to_render.expose_gizmo());
	objects_on_scene_bounds.push_back(*object_to_render.expose_bounds());

	primitives.push_back(object_to_render.expose_sdf_data());
}

void rendering_layer::on_update(timestep deltatime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_renderer->begin_scene(scene_camera);

	glm::vec3 light_normal = glm::normalize(main_light->get_normal());

	for (auto& obj : objects_on_scene)
	{
		scene_renderer->submit(obj, scene_camera, glm::mat4(1.0f));
		main_light->shine_on_objects(obj);

		if (show_objects_on_scene_gizmo)
		{
			gizmo g = *obj->expose_gizmo();
			g.set_position(obj->get_position());
			g.set_rotation(obj->get_rotation());
			g.render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());

			objects_on_scene_gizmo.push_back(g);
		}

		if (show_objects_on_scene_bounds)
		{
			bounds b = *obj->expose_bounds();
			b.set_position(obj->get_position());
			b.set_rotation(obj->get_rotation());
			b.set_scale(obj->get_scale());
			b.render(scene_camera->get_view_matrix(), scene_camera->get_projection_matrix());

			objects_on_scene_bounds.push_back(b);
		}

		scene_renderer->submit_sdf(obj, scene_camera, glm::mat4(1.0f), light_normal, glm::vec4(main_light->get_color(), 1), primitives);
	}
}