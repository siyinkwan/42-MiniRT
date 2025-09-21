/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:45:42 by sguan             #+#    #+#             */
/*   Updated: 2025/09/18 21:03:54 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


double	bump_func(double u, double v)
{
	
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
	perturbed_n = (perturbed_n, vec3_scale(bump.B, hv));
	return (perturbed_n);
}
