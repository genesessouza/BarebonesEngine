#include "gizmos_layer.h"

gizmos_layer::gizmos_layer(std::vector<entity*> scene_objs, perspective_camera& main_cam)
{
	objs_on_scene = scene_objs;

	for (auto* obj : objs_on_scene)
		gizmos_on_scene.push_back(*obj->expose_gizmo());

	scene_cam = &main_cam;
}

void gizmos_layer::on_update(timestep deltatime)
{
	for (auto* obj : objs_on_scene)
	{
		gizmo g = *obj->expose_gizmo();
		g.set_position(obj->get_position());
		g.set_rotation(obj->get_rotation());

		g.render(scene_cam->get_view_matrix(), scene_cam->get_projection_matrix());
	}
}

void gizmos_layer::on_event(event& e)
{

}