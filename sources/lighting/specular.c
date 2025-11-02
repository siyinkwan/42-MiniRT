/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:06:08 by sguan             #+#    #+#             */
/*   Updated: 2025/09/15 22:47:38 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	reflect(t_vec3 light_dir, t_vec3 normal)
{
	t_vec3	reflect_dir;
	double	dot;

	dot = vec3_dot(light_dir, normal);
	reflect_dir = vec3_subtract(light_dir, vec3_scale(normal, 2.0 * dot));
	return (reflect_dir);
}

t_vec3	calculate_specular(t_scene *scene, t_hit *hit)
{
	t_vec3	spec;
	t_vec3	l_dir;
	t_vec3	v_dir;
	double	s;
	int		i;
	
	spec = vec3_create(0, 0, 0);
	v_dir = vec3_normalize(vec3_subtract(scene->camera.position, hit->point));
	i = 0;
	while (i < scene->light_count)
	{
		l_dir = vec3_normalize(vec3_subtract(scene->lights[i].pos, hit->point));
		if (vec3_dot(hit->normal, l_dir) > 0.0
		&& vec3_dot(hit->normal, v_dir) > 0.0
		&& !is_in_shadow(scene, hit, scene->lights[i].pos))
		{
			s = pow(fmax(0.0, vec3_dot(reflect(vec3_scale(l_dir, -1.0), hit->normal),
			v_dir)), hit->material->shininess);
			s *= hit->material->specular * scene->lights[i].brightness;
			spec = vec3_add(spec, vec3_scale(scene->lights[i].color, s));
		}
		i++;
	}
	return (spec);
}
