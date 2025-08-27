/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:15:14 by sguan             #+#    #+#             */
/*   Updated: 2025/08/27 21:47:54 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	intersect_infinite_cylinder(t_ray ray, t_cylinder cylinder)
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

double	intersect_cylinder_caps(t_ray ray, t_cylinder cylinder)
{
	//reuse plane function
}

double	intersect_cylinder(t_ray ray, t_cylinder cylinder)
{
	double	t_side;
	double	t_cap;
	t_vec3	hit_point;
	double	y_min;
	double	y_max;

	t_side = intersect_infinite_cylinder(ray, cylinder);
	y_min = cylinder.center.y - cylinder.height / 2.0;
    y_max = cylinder.center.y + cylinder.height / 2.0;
	if (t_side > 0.0)
	{
		hit_point = ray_at(ray, t_side);
		if (hit_point.y >= y_min && hit_point.y <= y_max)
			return (t_side);
	}
	t_cap = intersect_cylinder_caps(ray, cylinder);
	return (t_cap);
}
