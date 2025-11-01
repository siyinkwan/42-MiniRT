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
	double		cos_sq;
	t_vec3		oc;
	double		dot_dir_axis;
	double		dot_oc_axis;

	cos_sq = cos(cone.angle) * cos(cone.angle);
	oc = vec3_subtract(ray.origin, cone.apex);
	dot_dir_axis = vec3_dot(ray.direction, cone.axis);
	dot_oc_axis = vec3_dot(oc, cone.axis);
	q.a = dot_dir_axis * dot_dir_axis - cos_sq;
	q.b = 2.0 * (dot_dir_axis * dot_oc_axis - vec3_dot(ray.direction, oc) * cos_sq);
	q.c = dot_oc_axis * dot_oc_axis - vec3_dot(oc, oc) * cos_sq;
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
	t_vec3	hit_point;
	t_vec3	base_center;

	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
	base.point = base_center;
	base.normal = cone.axis;
	base_radius = cone.height * tan(cone.angle);
	t_base = calculate_hit_plane(ray, base);
	
	if (t_base > 0.0)
	{
		hit_point = ray_at(ray, t_base);
		if (in_circle(hit_point, base.point, base.normal, base_radius))
			return (t_base);
	}
	return (-1.0);
}

double	calculate_hit_cone(t_ray ray, t_cone cone)
{
	double	closest_t;
	double	t_temp;
	t_vec3	hit_point;
	t_vec3	apex_to_hit;
	double	projection;

	closest_t = INFINITY;
	t_temp = cal_hit_infinite_cone(ray, cone);
	if (t_temp > 0.0)
	{
		hit_point = ray_at(ray, t_temp);
		apex_to_hit = vec3_subtract(hit_point, cone.apex);
		projection = vec3_dot(apex_to_hit, cone.axis);
		if (projection >= 0.0 && projection <= cone.height)
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
	t_vec3	apex_to_point;
	double	projection;
	t_vec3	projection_point;
	t_vec3	radial;
	t_vec3	normal;
	double	h;

	apex_to_point = vec3_subtract(point, cone->apex);
	projection = vec3_dot(apex_to_point, cone->axis);
	projection_point = vec3_add(cone->apex, vec3_scale(cone->axis, projection));
	radial = vec3_subtract(point, projection_point);
	radial = vec3_normalize(radial);
	h = projection * tan(cone->angle);
	if (h < EPSILON)
		h = EPSILON;
	normal = vec3_subtract(radial, vec3_scale(cone->axis, tan(cone->angle)));
	return (vec3_normalize(normal));
}

t_hit	intersect_cone(t_ray ray, t_cone cone)
{
	t_hit	result;
	double	t;
	t_vec3	base_center;
	double	dist_to_base;

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
		
		base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
		dist_to_base = vec3_distance(result.point, base_center);
		
		if (dist_to_base < cone.height * tan(cone.angle) + EPSILON)
			result.normal = cone.axis;
		else
			result.normal = calculate_cone_surface_normal(result.point, &cone);
		result.material = &cone.material;
	}
	return (result);
}
