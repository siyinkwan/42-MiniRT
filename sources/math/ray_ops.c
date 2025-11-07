/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:07:24 by sguan             #+#    #+#             */
/*   Updated: 2025/11/07 18:14:47 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_ray	ray_create(t_vec3 origin, t_vec3 direction)
{
	t_ray	ray;

	ray.origin = origin;
	ray.direction = vec3_normalize(direction);
	ray.t_min = 0.001;
	ray.t_max = INFINITY;
	return (ray);
}

bool	ray_valid_t(t_ray ray, double t)
{
	return (t >= ray.t_min && t <= ray.t_max);
}

t_vec3	ray_at(t_ray ray, double t)
{
	t_vec3	scaled_direction;
	t_vec3	result;

	scaled_direction = vec3_scale(ray.direction, t);
	result = vec3_add(ray.origin, scaled_direction);
	return (result);
}
