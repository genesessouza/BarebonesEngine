#include "physics_layer.h"
#include "physics.h"

physics_layer::physics_layer()
{

}

void physics_layer::add_object(entity& obj)
{
	entities_on_scene.push_back(&obj);
}

void physics_layer::on_update(timestep delta_time)
{
	for (auto& obj : entities_on_scene)
	{
		if (!obj->is_static_entity())
		{
			glm::vec3 current_position = obj->get_position();
			current_position += physics::gravity() * (float)delta_time;
			obj->set_position(current_position, world);
		}
	}
}