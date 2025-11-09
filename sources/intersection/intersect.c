/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 20:11:24 by sguan             #+#    #+#             */
/*   Updated: 2025/11/09 16:56:54 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_material(t_hit *hit, t_object *object)
{
	if (object->type == OBJECT_SPHERE)
		hit->material = &object->u_data.sphere.material;
	else if (object->type == OBJECT_PLANE)
		hit->material = &object->u_data.plane.material;
	else if (object->type == OBJECT_CYLINDER)
		hit->material = &object->u_data.cylinder.material;
	else if (object->type == OBJECT_CONE)
		hit->material = &object->u_data.cone.material;
}

t_hit	intersect_object(t_ray ray, t_object *object)
{
	t_hit	hit;
	t_hit	miss;

	miss.hit = false;
	miss.t = -1.0;
	miss.material = NULL;
	miss.object = NULL;
	if (!object)
		return (miss);
	if (object->type == OBJECT_SPHERE)
		hit = intersect_sphere(ray, object->u_data.sphere);
	else if (object->type == OBJECT_PLANE)
		hit = intersect_plane(ray, object->u_data.plane);
	else if (object->type == OBJECT_CYLINDER)
		hit = intersect_cylinder(ray, object->u_data.cylinder);
	else if (object->type == OBJECT_CONE)
		hit = intersect_cone(ray, object->u_data.cone);
	else
		return (miss);
	if (!hit.hit)
		return (hit);
	hit.object = object;
	set_material(&hit, object);
	hit.bump = NULL;
	return (hit);
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
			closest_hit = current_hit;
		current_object = current_object->next;
	}
	return (closest_hit);
}
