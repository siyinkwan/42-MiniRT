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

/*
** Smooth interpolation function (cosine interpolation)
*/
static double	smooth_interp(double a, double b, double t)
{
	double	ft;

	ft = t * 3.14159265;
	ft = (1.0 - cos(ft)) * 0.5;
	return (a * (1.0 - ft) + b * ft);
}

/*
** Hash function for pseudo-random values at grid points
*/
static double	hash2d(int x, int y)
{
	double	n;

	n = sin(x * 127.1 + y * 311.7) * 43758.5453;
	return (n - floor(n));
}

/*
** Smooth noise function with bilinear interpolation
*/
static double	smooth_noise(double x, double y)
{
	int		ix;
	int		iy;
	double	fx;
	double	fy;
	double	v1;
	double	v2;

	ix = (int)floor(x);
	iy = (int)floor(y);
	fx = x - ix;
	fy = y - iy;
	v1 = smooth_interp(hash2d(ix, iy), hash2d(ix + 1, iy), fx);
	v2 = smooth_interp(hash2d(ix, iy + 1), hash2d(ix + 1, iy + 1), fx);
	return (smooth_interp(v1, v2, fy));
}

/*
** Turbulent noise bump function - creates organic pattern
*/
double	bump_func(double u, double v)
{
	double	result;
	double	amp;
	double	freq;
	int		i;

	result = 0.0;
	amp = 1.0;
	freq = 1.0;
	i = 0;
	while (i < 6)
	{
		result += smooth_noise(u * freq * 5.0, v * freq * 5.0) * amp;
		amp *= 0.5;
		freq *= 2.0;
		i++;
	}
	return ((result - 0.5) * 20.0);
}

void	compute_uv(t_hit *t, double *u, double *v)
{
	t_vec3	d;
	t_vec3	local_u;
	t_vec3	local_v;
	t_vec3	local_pos;
	t_vec3	radial;
	double	height;

    if (t->object->type == OBJECT_SPHERE)
    {
        d = vec3_normalize(vec3_subtract(t->point,
                    t->object->data.sphere.center));
        *u = 0.5 + atan2(d.z, d.x) / (2.0 * PI);
        *v = 0.5 - asin(d.y) / PI;
    }
    else if (t->object->type == OBJECT_PLANE)
    {
        if (fabs(t->normal.y) > 0.9)
            local_u = vec3_create(1.0, 0.0, 0.0);
        else
            local_u = vec3_normalize(vec3_cross(t->normal, 
                vec3_create(0.0, 1.0, 0.0)));
        local_v = vec3_cross(t->normal, local_u);
        *u = vec3_dot(t->point, local_u);
        *v = vec3_dot(t->point, local_v);
    }
    else if (t->object->type == OBJECT_CYLINDER)
    {
        local_pos = vec3_subtract(t->point, t->object->data.cylinder.center);
        height = vec3_dot(local_pos, t->object->data.cylinder.axis);
        radial = vec3_subtract(local_pos, 
            vec3_scale(t->object->data.cylinder.axis, height));
        d = vec3_normalize(radial);
        *u = 0.5 + atan2(d.z, d.x) / (2.0 * PI);
        *v = height;
    }
    else if (t->object->type == OBJECT_CONE)
    {
        local_pos = vec3_subtract(t->point, t->object->data.cone.apex);
        height = vec3_dot(local_pos, t->object->data.cone.axis);
        radial = vec3_subtract(local_pos,
            vec3_scale(t->object->data.cone.axis, height));
        d = vec3_normalize(radial);
        *u = 0.5 + atan2(d.z, d.x) / (2.0 * PI);
        *v = height;
    }
    else
    {
        d = vec3_normalize(t->point);
        *u = atan2(d.z, d.x) / (2.0 * PI);
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
	t_bump	bump;
	double	h0;
	double	hu;
	double	hv;
	t_vec3	perturbed_n;

	if (!hit->material->bump)
		return (hit->normal);
	compute_uv(hit, &bump.u, &bump.v);
	compute_tangent_space(hit, &bump.T, &bump.B);
	h0 = bump_func(bump.u, bump.v);
	hu = (bump_func(bump.u + B_EPSILON, bump.v) - h0) / B_EPSILON;
	hv = (bump_func(bump.u, bump.v + B_EPSILON) - h0) / B_EPSILON;
	perturbed_n = vec3_subtract(hit->normal, vec3_scale(bump.T, hu));
	perturbed_n = vec3_subtract(perturbed_n, vec3_scale(bump.B, hv));
	return (vec3_normalize(perturbed_n));
}

