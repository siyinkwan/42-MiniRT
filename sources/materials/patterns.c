/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:30:25 by sguan             #+#    #+#             */
/*   Updated: 2025/10/29 16:37:18 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	checkerboard_pattern(double u, double v, t_vec3 color1, t_vec3 color2)
{
	int	check;

	check = (int)floor(u) + (int)floor(v);
	if (check % 2 == 0)
		return (color1);
	return (color2);
}

t_vec3	apply_pattern(t_hit *hit)
{
	double	u;
	double	v;

	if (hit->material->pattern_type == PATTERN_CHECKER)
	{
		compute_uv(hit, &u, &v);
		return (checkerboard_pattern(u, v,
				hit->material->color,
				hit->material->pattern_color));
	}
	return (hit->material->color);
}
