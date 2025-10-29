/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:03:28 by sguan             #+#    #+#             */
/*   Updated: 2025/09/15 22:46:07 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	calculate_diffuse(t_scene *scene, t_hit *hit)
{
	t_vec3	diffuse;
	t_vec3	l_dir;
	double	lambert;
	double	d;
	int		i;

	diffuse = vec3_create(0, 0, 0);
	i = 0;
	while (i < scene->light_count)
	{
		if (!is_in_shadow(scene, hit, scene->lights[i].pos))
		{
			l_dir = vec3_subtract(scene->lights[i].pos, hit->point);
			l_dir = vec3_normalize(l_dir);
			lambert = fmax(0.0, vec3_dot(hit->normal, l_dir));
			d = hit->material->diffuse * lambert * scene->lights[i].brightness;
			diffuse.x += scene->lights[i].color.x * hit->material->color.x * d;
			diffuse.y += scene->lights[i].color.y * hit->material->color.y * d;
			diffuse.z += scene->lights[i].color.z * hit->material->color.z * d;
		}
		i++;
	}
	return (diffuse);
}
