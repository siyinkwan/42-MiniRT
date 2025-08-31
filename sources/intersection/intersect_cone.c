/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:30:37 by sguan             #+#    #+#             */
/*   Updated: 2025/08/31 21:01:08 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	cal_hit_infinite_cone(t_ray ray, t_cone cone)
{
	t_quadratic	q;
	double		k;
	t_vec3		oc;
		
	k = tan(cone.angle);
	oc = vec3_subtract(ray.origin, cone.apex);
	q.a = ray.direction.x * ray.direction.x + 
			ray.direction.z * ray.direction.z - 
			k * k * ray.direction.y * ray.direction.y;
	q.b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z - 
					k * k * oc.y * ray.direction.y);       
	q.c = oc.x * oc.x + oc.z * oc.z - k * k * oc.y * oc.y;
	q.discriminant = q.b * q.b - 4 * q.a * q.c;
	if (fabs(q.a) < EPSILON || q.discriminant < 0)
		return (-1.0);
	else
	{
		q.t1 = (-q.b - sqrt(q.discriminant)) / (2 * q.a);
		q.t2 = (-q.b + sqrt(q.discriminant)) / (2 * q.a);
		if (ray_valid_t(ray, q.t1))
			return (q.t1);
		else if (ray_valid_t(ray, q.t2))
			return (q.t2);
	}
	return (-1.0);
}

double	cal_hit_cone_base(t_ray ray, t_cone cone)
{
	t_plane	base;
	double	t_base;
	double	base_radius;

	base.point = vec3_create(cone.apex.x,
			cone.apex.y + cone.height, cone.apex.z);
	base.normal = cone.axis;
	base_radius = cone.height * tan(cone.angle);
	t_base = calculate_hit_plane(ray, base);
	if (t_base > 0.0 && in_circle(t_base, ray, base, base_radius))
		return (t_base);
	return (-1.0);
}

double	calculate_hit_cone(t_ray ray, t_cone cone)
{
	double	closest_t;
	double	t_temp;
	t_vec3	hit_point;

	closest_t = INFINITY;
	t_temp = cal_hit_infinite_cone(ray, cone);
	if (t_temp > 0.0)
	{
		hit_point = ray_at(ray, t_temp);
		if (hit_point.y >= cone.apex.y && 
			hit_point.y <= (cone.apex.y + cone.height))
			closest_t = t_temp;
	}
	t_temp = cal_hit_cone_base(ray, cone);
	if (t_temp > 0.0 && t_temp < closest_t)
		closest_t = t_temp;
	if (closest_t < INFINITY)
		return (closest_t);
	else
		return (-1.0);
}

t_vec3	calculate_cone_surface_normal(t_vec3 point, t_cone *cone)
{
	double	k;
	t_vec3	local_point;
	t_vec3	normal;

	k = tan(cone->angle);
	local_point = vec3_subtract(point, cone->apex);
	normal = vec3_create(2.0 * local_point.x,
		-2.0 * k * k * local_point.y,
		2.0 * local_point.z);
	return (vec3_normalize(normal));
}

t_hit	intersect_cone(t_ray ray, t_cone cone)
{
	t_hit	result;
	double	t;

	result.hit = false;
	result.t = -1.0;
	result.material = NULL;
	result.object = NULL;
	t = calculate_hit_cone(ray, cone);
	if (t > 0.0)
	{
		result.hit = true;
		result.t = t;
		result.point = ray_at(ray, t);
		if (fabs(result.point.y - (cone.apex.y + cone.height)) < EPSILON)
			result.normal = cone.axis;
		else
			result.normal = calculate_cone_surface_normal(result.point, &cone);
		result.material = &cone.material;
	}
	return (result);
}
