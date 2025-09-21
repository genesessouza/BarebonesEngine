#include "physics_layer.h"

physics_layer::physics_layer()
{

}

void physics_layer::add_object(entity& obj)
{
	entities_on_scene.push_back(&obj);
}

void physics_layer::check_for_collisions(collider* other)
{
	for (auto entity : entities_on_scene)
	{
		if (entity->get_collider()->check_for_collision(other->get_obb()))
		{
			if (other != entity->get_collider())
				std::cout << "Collided!" << std::endl;
		}
	}
}

void physics_layer::on_update(timestep delta_time)
{
	for (auto* entity : entities_on_scene)
		check_for_collisions(entity->get_collider());
}