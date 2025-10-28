/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:45:42 by sguan             #+#    #+#             */
/*   Updated: 2025/09/30 08:43:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


double	bump_func(double u, double v)
{
	double	result;

	result = 10 * sin(u) * cos(v);
	return (result);
}

t_vec3	apply_bump(t_hit *hit)
{
	t_vec3	perturbed_n;
	t_bump	bump;
	double	h0;
	double	hu;
	double	hv;

	//n′≈n−du​T−dv​B
	h0 = bump_func(bump.u, bump.v);
	hu = (bump_func(bump.u + B_EPSILON, bump.v) - h0) / B_EPSILON;
	hv = (bump_func(bump.u, bump.v + B_EPSILON) - h0) / B_EPSILON;
	perturbed_n = vec3_subtract(hit->normal, vec3_scale(bump.T, hu));
	perturbed_n = vec3_subtract(perturbed_n, vec3_scale(bump.B, hv));
	return (perturbed_n);
	
}
