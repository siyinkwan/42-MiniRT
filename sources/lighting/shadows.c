/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:16:43 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 15:02:58 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	is_in_shadow(t_scene *scene, t_hit *hit, t_vec3 light_pos)
{
	t_vec3	to_light;
	t_vec3	shadow_origin;
	t_ray	shadow_ray;
	t_hit	blocker;
	double	distance;

	to_light = vec3_subtract(light_pos, hit->point);
	distance = vec3_length(to_light);
	shadow_origin = vec3_add(hit->point, vec3_scale(hit->normal, EPSILON));
	shadow_ray = ray_create(shadow_origin, to_light);
	blocker = intersect_scene(shadow_ray, scene);
	return (blocker.hit && blocker.t < distance);
}
