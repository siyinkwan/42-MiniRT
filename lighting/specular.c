/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:06:08 by sguan             #+#    #+#             */
/*   Updated: 2025/09/14 22:56:43 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	reflect(t_vec3 light_dir, t_vec3 normal)
{
	t_vec3	reflect_dir;
	t_vec3	projection;

	projection =  vec3_scale(normal, vec3_dot(light_dir, normal));
	reflect_dir = vec3_subtract(light_dir, vec3_scale(projection, 2));
	return (reflect_dir);
}

t_vec3	calculate_specular(t_scene *scene, t_hit *hit)
{
	t_vec3	specular;
	t_vec3	light_dir;
	t_vec3	view_dir;
	double	s;
	int		i;

	specular = vec3_create(0, 0, 0);
	view_dir = vec3_normalize(vec3_subtract(scene->camera.position, hit->point));
	i = 0;
	while (i < scene->light_count)
	{
		light_dir = vec3_normalize(vec3_subtract(scene->lights[i].position, hit->point));
		if (vec3_dot(hit->normal, light_dir) > 0.0
			&& vec3_dot(hit->normal, view_dir) > 0.0)
		{
			s = pow(fmax(0.0, vec3_dot(reflect(light_dir, hit->normal), view_dir)),
					hit->material->shininess);
			s *= hit->material->specular * scene->lights[i].brightness;
			specular = vec3_add(specular, vec3_scale(scene->lights[i].color, s));
		}
		i++;
	}
	return (specular);
}
