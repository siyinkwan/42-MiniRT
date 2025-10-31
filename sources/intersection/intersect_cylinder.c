/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:15:14 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 20:57:48 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minirt.h"

double	cal_hit_infinite_cylinder(t_ray ray, t_cylinder cylinder)
{
	t_quadratic	q;
	t_vec3		oc;
	t_vec3		axis_ray_cross;
	t_vec3		axis_oc_cross;
	double		axis_dot_ray;

	oc = vec3_subtract(ray.origin, cylinder.center);
	axis_dot_ray = vec3_dot(cylinder.axis, ray.direction);
	axis_ray_cross = vec3_subtract(ray.direction,
			vec3_scale(cylinder.axis, axis_dot_ray));
	axis_oc_cross = vec3_subtract(oc,
			vec3_scale(cylinder.axis, vec3_dot(cylinder.axis, oc)));
	q.a = vec3_dot(axis_ray_cross, axis_ray_cross);
	if (fabs(q.a) < EPSILON)
		return (-1.0);
	q.b = 2.0 * vec3_dot(axis_ray_cross, axis_oc_cross);
	q.c = vec3_dot(axis_oc_cross, axis_oc_cross)
		- cylinder.radius * cylinder.radius;
	q.discriminant = q.b * q.b - 4 * q.a * q.c;
	if (q.discriminant < 0)
		return (-1.0);
	q.t1 = (-q.b - sqrt(q.discriminant)) / (2 * q.a);
	q.t2 = (-q.b + sqrt(q.discriminant)) / (2 * q.a);
	if (ray_valid_t(ray, q.t1))
		return (q.t1);
	else if (ray_valid_t(ray, q.t2))
		return (q.t2);
	return (-1.0);
}

bool	in_circle(double t, t_ray ray, t_plane plane, double radius)
{
	t_vec3	hit;
	t_vec3	to_hit;
	t_vec3	n;
	t_vec3	on_plane;
	double	distance;

	// Project hit point onto the cap plane and measure radial distance.
	// Normalize normal to avoid scaling errors when plane.normal isn't unit-length.
	n = vec3_normalize(plane.normal);
	hit = ray_at(ray, t);
	to_hit = vec3_subtract(hit, plane.point);
	on_plane = vec3_subtract(to_hit, vec3_scale(n, vec3_dot(to_hit, n)));
	distance = vec3_length(on_plane);
	if (distance <= radius + 1e-6)
		return (1);
	return (0);
}

static double	calculate_cap_intersection(t_ray ray, t_plane plane)
{
	t_vec3	op;
	t_vec3	n;
	double	denominator;
	double	numerator;
	double	t;

	// Use a normalized cap normal for stable intersection and a slightly
	// relaxed parallel threshold to handle grazing views of the cap.
	n = vec3_normalize(plane.normal);
	op = vec3_subtract(plane.point, ray.origin);
	denominator = vec3_dot(ray.direction, n);
	if (fabs(denominator) > 1e-6)
	{
		numerator = vec3_dot(op, n);
		t = numerator / denominator;
		if (ray_valid_t(ray, t))
			return (t);
	}
	return (-1.0);
}

double	cal_hit_cylinder_caps(t_ray ray, t_cylinder cylinder)
{
	t_plane	top;
	t_plane	bottom;
	double	t_top;
	double	t_bottom;

	top.point = vec3_add(cylinder.center,
			vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom.point = vec3_subtract(cylinder.center,
			vec3_scale(cylinder.axis, cylinder.height / 2.0));
	top.normal = cylinder.axis;
	bottom.normal = vec3_scale(cylinder.axis, -1.0);
	t_top = calculate_cap_intersection(ray, top);
	t_bottom = calculate_cap_intersection(ray, bottom);
	if (t_top > 0.0 && in_circle(t_top, ray, top, cylinder.radius))
	{
		if (t_bottom > 0.0 && in_circle(t_bottom, ray, bottom, cylinder.radius))
		{
			if (t_top > t_bottom)
				return (t_bottom);
		}
		return (t_top);
	}
	if (t_bottom > 0.0 && in_circle(t_bottom, ray, bottom, cylinder.radius))
		return (t_bottom);
	return (-1.0);
}

double	calculate_hit_cylinder(t_ray ray, t_cylinder cylinder)
{
	double	closest_t;
	double	t_temp;
	t_vec3	hit_point;
	t_vec3	to_hit;
	double	projection;
    double  half_h;

	closest_t = INFINITY;
	half_h = cylinder.height / 2.0;
	t_temp = cal_hit_infinite_cylinder(ray, cylinder);
	if (t_temp > 0.0)
	{
		hit_point = ray_at(ray, t_temp);
		to_hit = vec3_subtract(hit_point, cylinder.center);
		projection = vec3_dot(to_hit, cylinder.axis);
		if (projection >= -half_h && projection <= half_h)
			closest_t = t_temp;
	}
	t_temp = cal_hit_cylinder_caps(ray, cylinder);
	// Prefer cap when the two intersections are numerically tied
	if (t_temp > 0.0 && (closest_t == INFINITY || t_temp + 1e-6 < closest_t
		|| fabs(t_temp - closest_t) <= 1e-6))
		closest_t = t_temp;
	if (closest_t < INFINITY)
		return (closest_t);
	return (-1.0);
}

t_hit	intersect_cylinder(t_ray ray, t_cylinder cylinder)
{
	t_hit	result;
	double	t;
	t_vec3	to_point;
	double	projection;
	t_vec3	axis_point;
	double	half_h;

	result.hit = false;
	result.t = -1.0;
	result.material = NULL;
	result.object = NULL;
	t = calculate_hit_cylinder(ray, cylinder);
	if (t > 0.0)
	{
		result.hit = true;
		result.t = t;
		result.point = ray_at(ray, t);
		to_point = vec3_subtract(result.point, cylinder.center);
		projection = vec3_dot(to_point, cylinder.axis);
		// Clamp projection to cap planes within a small tolerance
		half_h = cylinder.height / 2.0;
		if (projection > half_h - 1e-6 && projection < half_h + 1e-6)
			projection = half_h;
		else if (projection > -half_h - 1e-6 && projection < -half_h + 1e-6)
			projection = -half_h;
		if (fabs(projection - half_h) < 1e-6)
			result.normal = cylinder.axis;
		else if (fabs(projection + half_h) < 1e-6)
			result.normal = vec3_scale(cylinder.axis, -1.0);
		else
		{
			axis_point = vec3_add(cylinder.center,
				vec3_scale(cylinder.axis, projection));
			result.normal = vec3_normalize(vec3_subtract(result.point,
						axis_point));
		}
		result.material = &cylinder.material;
	}
	return (result);
}


/*
#include "minirt.h"

double	cal_hit_infinite_cylinder(t_ray ray, t_cylinder cylinder)
{
	t_quadratic	q;
	t_vec3		oc;

	oc = vec3_subtract(ray.origin, cylinder.center);
	q.a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
	if (fabs(q.a) < EPSILON)
		return (-1.0);
	q.b = 2.0 * (oc.x * ray.direction.x + oc.z * ray.direction.z);
	q.c = oc.x * oc.x + oc.z * oc.z - cylinder.radius * cylinder.radius;
	q.discriminant = q.b * q.b - 4 * q.a * q.c;
	if (q.discriminant < 0)
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

bool	in_circle(double t, t_ray ray, t_plane plane, double radius)
{
	t_vec3	hit;
	double distance;
	
	hit = ray_at(ray, t);
	distance = vec3_distance(hit, plane.point);
	if (distance <= radius)
		return (1);
	return (0);
}

double	cal_hit_cylinder_caps(t_ray ray, t_cylinder cylinder)
{
	t_plane	top;
	t_plane	bottom;
	double	t_top;
	double	t_bottom;

	top.point = vec3_create(cylinder.center.x,
			cylinder.center.y + cylinder.height / 2.0, cylinder.center.z);
	bottom.point = vec3_create(cylinder.center.x,
			cylinder.center.y - cylinder.height / 2.0, cylinder.center.z);
	top.normal = cylinder.axis;
	bottom.normal = cylinder.axis;
	t_top = calculate_hit_plane(ray, top);
	t_bottom = calculate_hit_plane(ray, bottom);
	if (t_top > 0.0 && in_circle(t_top, ray, top, cylinder.radius))
	{
		if (t_bottom > 0.0 && in_circle(t_bottom, ray, bottom, cylinder.radius))
		{
			if (t_top > t_bottom)
				return (t_bottom);
		}
		return (t_top);
	}
	if (t_bottom > 0.0 && in_circle(t_bottom, ray, bottom, cylinder.radius))
		return (t_bottom);
	return (-1.0);
}

double	calculate_hit_cylinder(t_ray ray, t_cylinder cylinder)
{
	double	closest_t;
	double	t_temp;
	t_vec3	hit_point;

	closest_t = INFINITY;
	t_temp = cal_hit_infinite_cylinder(ray, cylinder);
	if (t_temp > 0.0)
	{
		hit_point = ray_at(ray, t_temp);
		if (hit_point.y >= (cylinder.center.y - cylinder.height / 2.0) &&
			hit_point.y <= (cylinder.center.y + cylinder.height / 2.0))
				closest_t = t_temp;
	}
	t_temp = cal_hit_cylinder_caps(ray, cylinder);
	if (t_temp > 0.0 && t_temp < closest_t)
		closest_t = t_temp;
	if (closest_t < INFINITY)
		return (closest_t);
	else
		return (-1.0);
}

t_hit	intersect_cylinder(t_ray ray, t_cylinder cylinder)
{
	t_hit	result;
	double	t;
	double	half_height;

	result.hit = false;
	result.t = -1.0;
	result.material = NULL;
	result.object = NULL;
	t = calculate_hit_cylinder(ray, cylinder);
	if (t > 0.0)
	{
		result.hit = true;
		result.t = t;
		result.point = ray_at(ray, t);
		half_height = cylinder.height / 2.0;
		if (fabs(result.point.y - (cylinder.center.y + half_height)) < EPSILON)
			result.normal = vec3_create(0, 1, 0);
		else if (fabs(result.point.y - (cylinder.center.y - half_height)) < EPSILON)
			result.normal = vec3_create(0, -1, 0);
		else
			result.normal = vec3_normalize(vec3_create(result.point.x - cylinder.center.x,
				0, result.point.z - cylinder.center.z));
		result.material = &cylinder.material;
	}
	return (result);
}
	*/