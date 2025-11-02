/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:29:17 by sguan             #+#    #+#             */
/*   Updated: 2025/09/15 22:13:46 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_ambient(t_scene *scene, char **tokens)
{
	if (count_tokens(tokens) != 3)
		return (printf("Error\nAmbient requires 2 parameters\n"), false);
	scene->ambient.ratio = ft_atof(tokens[1]);
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (printf("Error\nAmbient ratio invalid\n"), false);
	if (!parse_color(tokens[2], &scene->ambient.color))
	{
		printf("Error\nInvalid ambient color format\n");
		return (false);
	}
	return (true);
}

bool	parse_camera(t_scene *scene, char **tokens)
{
	if (count_tokens(tokens) != 4)
		return (printf("Error\nCamera requires 3 parameters\n"), false);
	if (!parse_vec3(tokens[1], &scene->camera.position))
	{
		printf("Error\nInvalid camera position format\n");
		return (false);
	}
	if (!parse_vec3(tokens[2], &scene->camera.forward)
		|| !is_normalized(scene->camera.forward))
	{
		printf("Error\nInvalid camera direction format\n");
		return (false);
	}
	if (vec3_length(scene->camera.forward) < EPSILON)
	{
		printf("Error\nCamera direction cannot be zero vector\n");
		return (false);
	}
	scene->camera.fov = ft_atof(tokens[3]);
	if (scene->camera.fov <= 0.0 || scene->camera.fov >= 180.0)
	{
		printf("Error\nCamera FOV must be between 0 and 180 degrees\n");
		return (false);
	}
	return (true);
}

void	add_light(t_scene *scene, t_light *new_lights)
{
	int		i;

	i = 0;
	while (i < scene->light_count)
	{
		new_lights[i] = scene->lights[i];
		i++;
	}
}

bool	parse_light(t_scene *scene, char **tokens)
{
	t_vec3	pos;
	double	brightness;
	t_vec3	color;
	t_light	*new_lights;

	if (count_tokens(tokens) != 4)
		return (printf("Error\nLight requires 3 parameters\n"), false);
	if (!parse_vec3(tokens[1], &pos))
		return(printf("Error\nInvalid light position\n"), false);
	brightness = ft_atof(tokens[2]);
	if (brightness < 0.0 || brightness > 1.0)
		return (printf("Error\nInvalid light brightness\n"), false);
	if (!parse_color(tokens[3], &color))
		return (printf("Error\nInvalid light color\n"), false);
	new_lights = malloc(sizeof(t_light) * (scene->light_count + 1));
	if (!new_lights)
		return (printf("Error\nMemory allocation failed\n"), false);
	add_light(scene, new_lights);
	new_lights[scene->light_count].pos = pos;
	new_lights[scene->light_count].brightness = brightness;
	new_lights[scene->light_count].color = color;
	free(scene->lights);
	scene->lights = new_lights;
	scene->light_count++;
	return (true);
}
