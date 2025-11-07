/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:59:38 by sguan             #+#    #+#             */
/*   Updated: 2025/11/07 17:05:01 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	calculate_cone_quadratic(t_ray ray, t_cone cone, t_quadratic *q)
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

t_vec3	calculate_cone_normal(t_cone cone, t_vec3 point, double h)
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

bool	in_circle(t_vec3 p, t_vec3 c, t_vec3 axis, double r)
{
	t_vec3	v;
	t_vec3	proj;

	v = vec3_subtract(p, c);
	proj = vec3_subtract(v, vec3_scale(axis, vec3_dot(v, axis)));
	return (vec3_length(proj) <= r + EPSILON);
}

