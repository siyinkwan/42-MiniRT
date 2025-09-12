/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:03:28 by sguan             #+#    #+#             */
/*   Updated: 2025/09/12 23:24:29 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	calculate_diffuse(t_hit *hit, t_scene *scene)
{
	t_vec3	diffuse;
	t_vec3	light_contribution;
	t_vec3	light_dir;
	double	lambertian;
	int		i;

	i = 0;
	while (i < scene->light_count)
	{
		light_dir = vec3_normalize(vec3_subtract(scene->lights[i].position, hit->point));
		lambertian = fmax(0.0, vec3_dot(hit->normal, light_dir));	
		light_contribution.x = scene->lights[i].color.x * hit->material->color.x
			* hit->material->diffuse * lambertian * scene->lights[i].brightness / 255.0;
		light_contribution.y = scene->lights[i].color.y * hit->material->color.y
			* hit->material->diffuse * lambertian * scene->lights[i].brightness / 255.0;
		light_contribution.z = scene->lights[i].color.z * hit->material->color.z
			* hit->material->diffuse * lambertian * scene->lights[i].brightness / 255.0;
		diffuse = vec3_add(diffuse, light_contribution);
		i++;
	}
	return (diffuse);
}
