/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:29:17 by sguan             #+#    #+#             */
/*   Updated: 2025/09/06 19:44:01 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_ambient(t_scene *scene, char **tokens)
{
	if (count_tokens(tokens) != 3)
		return (printf("Error: Ambient requires 2 parameters\n"), false);
	scene->ambient.ratio = ft_atof(tokens[1]);
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (printf("Error: Ambient ratio invalid\n"), false);
	if (!parse_color(tokens[2], &scene->ambient.color))
	{
		printf("Error: Invalid ambient color format\n");
		return (false);
	}
	return (true);
}

bool	parse_camera(t_scene *scene, char **tokens)
{
	if (count_tokens(tokens) != 4)
		return (printf("Error: Camera requires 3 parameters\n"), false);
	if (!parse_vec3(tokens[1], &scene->camera.position))
	{
		printf("Error: Invalid camera position format\n");
		return (false);
	}
	if (!parse_vec3(tokens[2], &scene->camera.forward))
	{
		printf("Error: Invalid camera direction format\n");
		return (false);
	}
	if (vec3_length(scene->camera.forward) < EPSILON)
	{
		printf("Error: Camera direction cannot be zero vector\n");
		return (false);
	}
	scene->camera.fov = ft_atof(tokens[3]);
	if (scene->camera.fov <= 0.0 || scene->camera.fov >= 180.0)
	{
		printf("Error: Camera FOV must be between 0 and 180 degrees\n");
		return (false);
	}
	return (true);
}


// parse_light()
