/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:45:42 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 13:54:56 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


double	bump_func(double u, double v)
{
	double	result;

	result = 10 * sin(u) * cos(v);
	return (result);
}

void	compute_uv(t_hit *t, double *u, double *v)
{
	t_vec3	d;

    if (t->object->type == OBJECT_SPHERE)
    {
        d = vec3_normalize(vec3_subtract(t->point,
                    t->object->data.sphere.center));
        *u = 0.5 + atan2(d.z, d.x) / (2.0 * PI);
        *v = 0.5 - asin(d.y) / PI;
    }
    else if (t->object->type == OBJECT_PLANE)
    {
        *u = t->point.x;
        *v = t->point.z;
    }
    else
    {
        d = vec3_normalize(t->point);
        *u = atan2(d.z, d.x) / (2.0 * PI);
        if (t->object->type == OBJECT_CYLINDER)
            *v = t->point.y;
        else
            *v = vec3_length(t->point);
    }
	*u *= t->material->pattern_scale;
	*v *= t->material->pattern_scale;
}

void	compute_tangent_space(t_hit *hit, t_vec3 *T, t_vec3 *B)
{
	t_vec3	up;

	up = vec3_create(0.0, 1.0, 0.0);
	if (fabs(vec3_dot(hit->normal, up)) > 0.99)
		up = vec3_create(1.0, 0.0, 0.0);
	*T = vec3_normalize(vec3_cross(up, hit->normal));
	*B = vec3_cross(hit->normal, *T);
}

t_vec3	apply_bump(t_hit *hit)
{
	t_vec3	perturbed_n;
	t_bump	bump;
	double	h0;
	double	hu;
	double	hv;

	h0 = bump_func(bump.u, bump.v);
	hu = (bump_func(bump.u + B_EPSILON, bump.v) - h0) / B_EPSILON;
	hv = (bump_func(bump.u, bump.v + B_EPSILON) - h0) / B_EPSILON;
	perturbed_n = vec3_subtract(hit->normal, vec3_scale(bump.T, hu));
	perturbed_n = vec3_subtract(perturbed_n, vec3_scale(bump.B, hv));
	return (perturbed_n);
}
