/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:06:08 by sguan             #+#    #+#             */
/*   Updated: 2025/09/14 19:51:23 by sguan            ###   ########.fr       */
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
	double	spec_intensity;
	int		i;

	specular = vec3_create(0, 0 , 0);
	i = 0;
	while (i < scene->light_count)
	{
		light_dir = vec3_normalize(vec3_subtract(scene->lights[i].position, hit->point));
		view_dir = vec3_normalize(vec3_subtract(scene->camera.position, hit->point));
		spec_intensity = pow(fmax(0, vec3_dot(reflect(light_dir, hit->normal), view_dir)), 
			hit->material->shininess);
		specular.x = specular.x + hit->material->specular * spec_intensity
			* scene->lights[i].color.x * scene->lights[i].brightness; 
		specular.y = specular.y + hit->material->specular * spec_intensity
			* scene->lights[i].color.y * scene->lights[i].brightness;
		specular.z = specular.z + hit->material->specular * spec_intensity
			* scene->lights[i].color.z * scene->lights[i].brightness; 
		i++;
	}
	return (specular);
}
