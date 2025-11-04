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

static void	calculate_cone_quadratic(t_ray ray, t_cone cone, t_quadratic *q)
{
	t_vec3	oc;
	double	dot_dir_axis;
	double	dot_oc_axis;
	double	k;

	k = pow(tan(cone.angle), 2);
	oc = vec3_subtract(ray.origin, cone.apex);
	dot_dir_axis = vec3_dot(ray.direction, cone.axis);
	dot_oc_axis = vec3_dot(oc, cone.axis);
	q->a = vec3_dot(ray.direction, ray.direction)
		- (1 + k) * pow(dot_dir_axis, 2);
	q->b = 2 * (vec3_dot(ray.direction, oc)
			- (1 + k) * dot_dir_axis * dot_oc_axis);
	q->c = vec3_dot(oc, oc) - (1 + k) * pow(dot_oc_axis, 2);
	q->discriminant = q->b * q->b - 4 * q->a * q->c;
}

static double	solve_cone_intersection(t_quadratic q)
{
	double	t;
	double	t1;

	if (fabs(q.a) < EPSILON || q.discriminant < 0)
		return (-1.0);
	t = (-q.b - sqrt(q.discriminant)) / (2 * q.a);
	t1 = (-q.b + sqrt(q.discriminant)) / (2 * q.a);
	if (t > t1 && t1 > EPSILON)
		t = t1;
	if (t < EPSILON)
		return (-1.0);
	return (t);
}

static t_vec3	calculate_cone_normal(t_cone cone, t_vec3 point, double h)
{
	t_vec3	tip_to_p;
	t_vec3	perp;
	t_vec3	normal;
	double	radius;
	double	hyp;

	radius = cone.height * tan(cone.angle);
	hyp = sqrt(radius * radius + cone.height * cone.height);
	tip_to_p = vec3_subtract(point, cone.apex);
	perp = vec3_subtract(tip_to_p, vec3_scale(cone.axis, h));
	perp = vec3_normalize(perp);
	normal = vec3_subtract(vec3_scale(perp, cone.height / hyp),
			vec3_scale(cone.axis, radius / hyp));
	return (vec3_normalize(normal));
}

static bool	cone_body_hit(t_cone cone, t_ray ray, t_hit *hit)
{
	t_quadratic	q;
	double		t;
	t_vec3		point;
	t_vec3		tip_to_p;
	double		h;

	calculate_cone_quadratic(ray, cone, &q);
	t = solve_cone_intersection(q);
	if (t < 0)
		return (false);
	point = ray_at(ray, t);
	tip_to_p = vec3_subtract(point, cone.apex);
	h = vec3_dot(tip_to_p, cone.axis);
	if (h < 0 || h > cone.height || (hit->hit && t >= hit->t))
		return (false);
	hit->hit = true;
	hit->t = t;
	hit->point = point;
	hit->normal = calculate_cone_normal(cone, point, h);
	if (vec3_dot(hit->normal, ray.direction) > 0.0)
		hit->normal = vec3_scale(hit->normal, -1.0);
	hit->material = &cone.material;
	hit->object = NULL;
	return (true);
}

static bool	update_base_hit(t_hit *hit, double t, t_vec3 point, t_cone cone)
{
	if (!hit->hit || t < hit->t)
	{
		hit->hit = true;
		hit->t = t;
		hit->point = point;
		hit->normal = cone.axis;
		if (vec3_dot(hit->normal, vec3_subtract(cone.apex, point)) < 0)
			hit->normal = vec3_scale(hit->normal, -1.0);
		hit->material = &cone.material;
		hit->object = NULL;
		return (true);
	}
	return (false);
}

static bool	cone_base_hit(t_cone cone, t_ray ray, t_hit *hit)
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
	if (t_base <= 0.0)
		return (false);
	hit_point = ray_at(ray, t_base);
	if (!in_circle(hit_point, base.point, base.normal, base_radius))
		return (false);
	return (update_base_hit(hit, t_base, hit_point, cone));
}

t_hit	intersect_cone(t_ray ray, t_cone cone)
{
	t_hit	hit;

	hit.hit = false;
	hit.t = INFINITY;
	hit.material = NULL;
	hit.object = NULL;
	cone_body_hit(cone, ray, &hit);
	cone_base_hit(cone, ray, &hit);
	if (!hit.hit)
		hit.t = -1.0;
	return (hit);
}
