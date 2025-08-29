/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sphere.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:41:16 by sguan             #+#    #+#             */
/*   Updated: 2025/08/29 17:01:44 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt"

double	calculate_hit_sphere(t_ray ray, t_sphere sphere)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	oc = vec3_subtract(ray.origin, sphere.center);
	a = 1.0;
	b = 2.0 * vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	else if (discriminant == 0)
		return (-b / (2 * a));
	else
	{
		if ((-b - sqrt(discriminant)) / (2 * a) > 0)
			return ((-b - sqrt(discriminant)) / (2 * a));
		else if ((-b + sqrt(discriminant)) / (2 * a) > 0)
			return ((-b + sqrt(discriminant)) / (2 * a));
		else
			return (-1.0);
	}
}

t_hit		intersect_sphere(t_ray ray, t_sphere sphere)
{
	t_hit	result;
	double	t;
	t_vec3	to_sphere;

    result.hit = false;
    result.t = -1.0;
    result.material = NULL;
    result.object = NULL;
	t = calculate_hit_sphere(ray, sphere);
	if (t > 0.0)
	{
		result.hit = true;
		result.t = t;
		result.point = ray_at(ray, t);
		to_sphere = vec3_substract(result.point, sphere.center);
		result.normal = vec3_normalize(to_sphere);
		result.material = &sphere.material;
	}
	return (result);
}
