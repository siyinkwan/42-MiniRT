/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:15:14 by sguan             #+#    #+#             */
/*   Updated: 2025/11/09 17:32:20 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	cal_hit_infinite_cylinder(t_ray ray, t_cylinder cyl)
{
	t_quadratic	q;
	t_vec3		oc;
	t_vec3		d;
	t_vec3		delta;

	oc = vec3_subtract(ray.origin, cyl.center);
	d = vec3_subtract(ray.direction, vec3_scale(cyl.axis,
				vec3_dot(ray.direction, cyl.axis)));
	delta = vec3_subtract(oc, vec3_scale(cyl.axis, vec3_dot(oc, cyl.axis)));
	q.a = vec3_dot(d, d);
	q.b = 2.0 * vec3_dot(d, delta);
	q.c = vec3_dot(delta, delta) - cyl.radius * cyl.radius;
	q.discriminant = q.b * q.b - 4 * q.a * q.c;
	if (q.discriminant < 0 || fabs(q.a) < EPSILON)
		return (-1.0);
	q.t1 = (-q.b - sqrt(q.discriminant)) / (2 * q.a);
	q.t2 = (-q.b + sqrt(q.discriminant)) / (2 * q.a);
	if (ray_valid_t(ray, q.t1))
		return (q.t1);
	if (ray_valid_t(ray, q.t2))
		return (q.t2);
	return (-1.0);
}

double	cal_hit_cylinder_caps(t_ray ray, t_cylinder cyl)
{
	t_plane	top;
	t_plane	bottom;
	double	t_top;
	double	t_bottom;
	double	t_cap;

	top.point = vec3_add(cyl.center, vec3_scale(cyl.axis, cyl.height / 2.0));
	top.normal = cyl.axis;
	bottom.point = vec3_subtract(cyl.center,
			vec3_scale(cyl.axis, cyl.height / 2.0));
	bottom.normal = cyl.axis;
	t_top = calculate_hit_plane(ray, top);
	t_bottom = calculate_hit_plane(ray, bottom);
	t_cap = INFINITY;
	if (t_top > 0.0 && in_circle(ray_at(ray, t_top),
			top.point, cyl.axis, cyl.radius))
		t_cap = fmin(t_cap, t_top);
	if (t_bottom > 0.0 && in_circle(ray_at(ray, t_bottom),
			bottom.point, cyl.axis, cyl.radius))
		t_cap = fmin(t_cap, t_bottom);
	if (t_cap < INFINITY)
		return (t_cap);
	return (-1.0);
}

double	calculate_hit_cylinder(t_ray ray, t_cylinder cyl)
{
	double	t;
	double	closest_t;
	t_vec3	p;
	t_vec3	v;
	double	proj;

	closest_t = INFINITY;
	t = cal_hit_infinite_cylinder(ray, cyl);
	if (t > 0.0)
	{
		p = ray_at(ray, t);
		v = vec3_subtract(p, cyl.center);
		proj = vec3_dot(v, cyl.axis);
		if (proj >= -cyl.height / 2.0 && proj <= cyl.height / 2.0)
			closest_t = t;
	}
	t = cal_hit_cylinder_caps(ray, cyl);
	if (t > 0.0 && t < closest_t)
		closest_t = t;
	if (closest_t < INFINITY)
		return (closest_t);
	return (-1.0);
}

static t_vec3	calculate_cylinder_normal(t_cylinder cyl, t_vec3 point)
{
	t_vec3	v;
	t_vec3	radial;
	double	proj;
	double	hh;

	v = vec3_subtract(point, cyl.center);
	proj = vec3_dot(v, cyl.axis);
	hh = cyl.height / 2.0;
	if (fabs(proj - hh) < EPSILON)
		return (cyl.axis);
	else if (fabs(proj + hh) < EPSILON)
		return (vec3_scale(cyl.axis, -1));
	else
	{
		radial = vec3_subtract(v, vec3_scale(cyl.axis, proj));
		return (vec3_normalize(radial));
	}
}

t_hit	intersect_cylinder(t_ray ray, t_cylinder cyl)
{
	t_hit	result;

	result.hit = false;
	result.t = -1.0;
	result.material = NULL;
	result.object = NULL;
	result.t = calculate_hit_cylinder(ray, cyl);
	if (result.t <= 0.0)
		return (result);
	result.hit = true;
	result.point = ray_at(ray, result.t);
	result.normal = calculate_cylinder_normal(cyl, result.point);
	result.material = &cyl.material;
	result.object = (t_object *)&cyl;
	return (result);
}
