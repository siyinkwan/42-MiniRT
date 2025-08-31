/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_scene.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:11:24 by sguan             #+#    #+#             */
/*   Updated: 2025/08/31 21:28:37 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_hit	intersect_object(t_ray ray, t_object *object)
{
	t_hit	miss;

	miss.hit = false;
	miss.t = -1.0;
	miss.material = NULL;
	miss.object = NULL;
	if (!object)
		return (miss);   
	if (object->type == OBJECT_SPHERE)
		return (intersect_sphere(ray, object->data.sphere));
	else if (object->type == OBJECT_PLANE)
		return (intersect_plane(ray, object->data.plane));
	else if (object->type == OBJECT_CYLINDER)
		return (intersect_cylinder(ray, object->data.cylinder));
	else if (object->type == OBJECT_CONE)
		return (intersect_cone(ray, object->data.cone));
	return (miss);
}

t_hit	intersect_scene(t_ray ray, t_scene *scene)
{
	t_hit		closest_hit;
	t_hit		current_hit;
	t_object	*current_object;

	closest_hit.hit = false;
	closest_hit.t = INFINITY;
	closest_hit.material = NULL;
	closest_hit.object = NULL;
	if (!scene || !scene->objects)
		return (closest_hit);
	current_object = scene->objects;
	while (current_object != NULL)
	{
		current_hit = intersect_object(ray, current_object);
		if (current_hit.hit && current_hit.t < closest_hit.t
		&& current_hit.t > EPSILON)
		{
			closest_hit = current_hit;
			closest_hit.object = current_object;
		}
		current_object = current_object->next;
	}
	return (closest_hit);
}
