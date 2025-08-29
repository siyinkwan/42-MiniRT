/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:14:22 by sguan             #+#    #+#             */
/*   Updated: 2025/08/29 17:03:36 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	calculate_hit_plane(t_ray ray, t_plane plane)
{
	t_vec3	op;
	double	denominator;
	double	numerator;
	double	t;

	op = vec3_subtract(plane.point, ray.origin);
	denominator = vec3_dot(ray.direction, plane.normal);
	if (fabs(denominator) > EPSILON)
	{
		numerator = vec3_dot(op, plane.normal);
		t = numerator / denominator;
		if (ray_valid_t(ray, t))
			return (t);
	}
	return (-1.0);
}

t_hit		intersect_plane(t_ray ray, t_plane plane)
{
	t_hit	result;
	double	t;
	t_vec3	to_sphere;

    result.hit = false;
    result.t = -1.0;
    result.material = NULL;
    result.object = NULL;
	if (t > 0.0)
	{
		
		t = calculate_hit_plane(ray, plane);
		
	}
}