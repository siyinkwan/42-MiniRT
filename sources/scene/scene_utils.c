/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:29:36 by sguan             #+#    #+#             */
/*   Updated: 2025/10/31 14:45:28 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool validate_scene(t_scene *scene)
{
	if (!scene)
		return (false);
	if (scene->ambient.ratio < 0)
	{
		printf("Error\nMissing ambient light definition\n");
		return (false);
	}
	if (scene->camera.fov <= 0)
	{
		printf("Error\nMissing camera definition\n");
		return (false);
	}
	if (!scene->objects)
	{
		printf("Error\nScene must contain at least one object\n");
		return (false);
	}
	return (true);
}

void	error_exit(char *msg)
{
	printf("Error\n%s\n", msg);
	exit(1);
}

void	free_tokens(char **tokens)
{
    int	i;

    if (!tokens)
        return;
    i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}


void	free_scene(t_scene *scene)
{
	t_object	*current;
	t_object	*next;

	if (!scene)
		return ;
	current = scene->objects;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	scene->objects = NULL;
	if (scene->lights)
	{
		free(scene->lights);
		scene->lights = NULL;
	}
	scene->light_count = 0;
	free(scene);
}

////////////////////// FOR DEBUGGING/////////////////////////////////////

void	print_scene_info(t_scene *scene)
{
	t_object *cur;

	if (!scene)
		return ;
	printf("Camera pos: (%f,%f,%f), forward: (%f,%f,%f), aspect: %f\n",
		scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.forward.x,
		scene->camera.forward.y, scene->camera.forward.z,
		scene->camera.aspect_ratio);
	cur = scene->objects;
	while (cur)
	{
		if (cur->type == OBJECT_SPHERE)
			printf("Object: SPHERE center=(%f,%f,%f) radius=%f\n",
				cur->data.sphere.center.x, cur->data.sphere.center.y,
				cur->data.sphere.center.z, cur->data.sphere.radius);
		else if (cur->type == OBJECT_PLANE)
			printf("Object: PLANE point=(%f,%f,%f) normal=(%f,%f,%f)\n",
				cur->data.plane.point.x, cur->data.plane.point.y,
				cur->data.plane.point.z, cur->data.plane.normal.x,
				cur->data.plane.normal.y, cur->data.plane.normal.z);
		else if (cur->type == OBJECT_CYLINDER)
			printf("Object: CYLINDER center=(%f,%f,%f) axis=(%f,%f,%f) radius=%f height=%f\n",
				cur->data.cylinder.center.x, cur->data.cylinder.center.y,
				cur->data.cylinder.center.z, cur->data.cylinder.axis.x,
				cur->data.cylinder.axis.y, cur->data.cylinder.axis.z,
				cur->data.cylinder.radius, cur->data.cylinder.height);
		else if (cur->type == OBJECT_CONE)
			printf("Object: CONE apex=(%f,%f,%f) axis=(%f,%f,%f) angle=%f height=%f\n",
				cur->data.cone.apex.x, cur->data.cone.apex.y,
				cur->data.cone.apex.z, cur->data.cone.axis.x,
				cur->data.cone.axis.y, cur->data.cone.axis.z,
				cur->data.cone.angle, cur->data.cone.height);
		cur = cur->next;
	}
}
