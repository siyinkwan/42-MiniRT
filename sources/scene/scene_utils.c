/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:29:36 by sguan             #+#    #+#             */
/*   Updated: 2025/09/05 20:28:09 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	exit(1);
}

void	free_scene(t_scene *scene)
{
	
}

t_scene	*init_scene(void)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->ambient.ratio = 0.0;
	scene->ambient.color = vec3_create(0.0, 0.0, 0.0);
	scene->camera.position = vec3_create(0.0, 0.0, 0.0);
	scene->camera.forward = vec3_create(0.0, 0.0, -1.0);
	scene->camera.up = vec3_create(0.0, 1.0, 0.0);
	scene->camera.right = vec3_create(1.0, 0.0, 0.0);
	scene->camera.fov = 0.0;
	scene->camera.aspect_ratio = 0.0;
	scene->lights = NULL;
	scene->light_count = 0;
	scene->objects = NULL;
	scene->background = vec3_create(0.0, 0.0, 0.0);
	return (scene);
}
